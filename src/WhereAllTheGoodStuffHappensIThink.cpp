#include <Geode/modify/CameraTriggerGameObject.hpp>
#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "Manager.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

class $modify(MyLevelAreaInnerLayer, LevelAreaInnerLayer) {
	bool init(bool returning) {
		if (!LevelAreaInnerLayer::init(returning)) return false;
		if (returning) log::info("returning from a tower level");
		else {
			log::info("entering the tower from elsewhere");
			GameLevelManager* glm = GameLevelManager::get();
			if (!glm) return true;
			for (const auto&[robtopID, colonID] : Manager::getSharedInstance()->robtopToColon) glm->downloadLevel(colonID, false);
		}
		return true;
	}
	void onDoor(CCObject* sender) {
		if (!sender) return LevelAreaInnerLayer::onDoor(sender);
		const int robtopsID = sender->getTag();
		if (robtopsID < 5001 || robtopsID > 5004) return LevelAreaInnerLayer::onDoor(sender);

		const std::unordered_map<int, int>& robToColon = Manager::getSharedInstance()->robtopToColon;
		if (!robToColon.contains(robtopsID)) return LevelAreaInnerLayer::onDoor(sender);
		const int colonsID = robToColon.find(robtopsID)->second;
		GJGameLevel* colonsVersion = GameLevelManager::get()->getSavedLevel(colonsID);
		if (!colonsVersion) {
			Utils::logErrorCustomFormat("GJGameLevel", robtopsID, colonsID);
			return LevelAreaInnerLayer::onDoor(sender);
		}

		colonsVersion->setUserObject("colon-variant"_spr, CCBool::create(true));
		Manager::getSharedInstance()->useCanonSpawn = colonsID == THE_DEEP_SEWERS;

		CCInteger* robtopsIDAsCCObject = CCInteger::create(robtopsID);
		robtopsIDAsCCObject->setTag(robtopsID);
		colonsVersion->setUserObject("original-robtop-ID"_spr, robtopsIDAsCCObject);

		FMODAudioEngine::get()->playEffect("playSound_01.ogg"); // since we're not calling the original function, mimic vanilla behavior with SFX
		CCScene* playScene = PlayLayer::scene(colonsVersion, false, false);
		if (!playScene) {
			Utils::logErrorCustomFormat("CCScene from calling PlayLayer::scene", robtopsID, colonsID);
			return LevelAreaInnerLayer::onDoor(sender);
		}

		CCTransitionFade* transition = CCTransitionFade::create(0.5f, playScene);
		if (!transition) {
			Utils::logErrorCustomFormat("CCTransitionFade", robtopsID, colonsID);
			return LevelAreaInnerLayer::onDoor(sender);
		}

		CCDirector::sharedDirector()->replaceScene(transition);
		log::info("pushing scene to level {}", colonsID);
	}
};

class $modify(MyGameManager, GameManager) {
	void returnToLastScene(GJGameLevel* level) {
		if (!level || !level->getUserObject("colon-variant"_spr)) return GameManager::returnToLastScene(level);
		const int colonsID = level->m_levelID.value();
		const int robtopsID = level->getUserObject("original-robtop-ID"_spr)->getTag();
		if (colonsID == THE_DEEP_SEWERS && !Manager::getSharedInstance()->useCanonSpawn) return GameManager::returnToLastScene(level);
		Manager::getSharedInstance()->useCanonSpawn = false;
		CCScene* levelAreaInnerLayer = LevelAreaInnerLayer::scene(true);
		if (!levelAreaInnerLayer) {
			Utils::logErrorCustomFormat("LevelAreaInnerLayer", robtopsID, colonsID);
			return GameManager::returnToLastScene(level);
		}
		CCTransitionFade* transition = CCTransitionFade::create(0.5f, levelAreaInnerLayer);
		if (!transition) {
			Utils::logErrorCustomFormat("CCTransitionFade", robtopsID, colonsID);
			return GameManager::returnToLastScene(level);
		}
		CCDirector::sharedDirector()->replaceScene(transition); // safely free PlayLayer to avoid bugs
		log::info("replacing scene with LevelAreaInnerLayer");
		GameManager::fadeInMenuMusic(); // mimic vanilla behavior
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	void setupHasCompleted() {
		if (this->m_level->m_levelID.value() != THE_DEEP_SEWERS || !this->m_levelSettings || this->m_levelSettings->m_spawnGroup != 79 || !this->m_level->getUserObject("colon-variant"_spr)) return PlayLayer::setupHasCompleted();
		this->m_levelSettings->m_spawnGroup = 81;
		PlayLayer::setupHasCompleted();
	}
	void startGame() {
		// this is really just for debug purposes.
		// the quote is just filler; ADHD go brrr.
		// --raydeeux
		if (!this->m_level || !CCScene::get()) return PlayLayer::startGame();
		CCLabelBMFont* wicklineLabel = CCLabelBMFont::create(fmt::format("\"if any of you ever come for my man i'll break a ***** off like a kitkat bar\"\n- Jane Wickline, 2025\n(canonPosition: {}, !colonVariant: {})", Manager::getSharedInstance()->useCanonSpawn, !this->m_level->getUserObject("colon-variant"_spr)).c_str(), "bigFont.fnt");
		wicklineLabel->setID("jane-wickline-debug-label"_spr);
		CCScene::get()->addChild(wicklineLabel);
		wicklineLabel->limitLabelWidth(400.f, 2.f, 0.001f);
		wicklineLabel->setPosition(CCScene::get()->getContentSize() / 2.f);
		PlayLayer::startGame();
	}
};

class $modify(MyEffectGameObject, EffectGameObject) {
	void triggerObject(GJBaseGameLayer* gjbgl, int p1, gd::vector<int> const* p2) {
		if (!Manager::getSharedInstance()->useCanonSpawn || !PlayLayer::get() || !PlayLayer::get()->m_level || PlayLayer::get()->m_level->m_levelID.value() != THE_DEEP_SEWERS) return EffectGameObject::triggerObject(gjbgl, p1, p2);
		for (int i = 0; i < this->m_groupCount; i++) {
			if (this->m_groups->at(i) != 900) continue;
			return log::info("[EFFECT] since we're using the canonical spawn location instead of the one on boomlings, disable trigger of ID {} targeting group {}", this->m_objectID, this->m_targetGroupID);
		}
		EffectGameObject::triggerObject(gjbgl, p1, p2);
	}
};

class $modify(MyCameraTriggerGameObject, CameraTriggerGameObject) {
	void triggerObject(GJBaseGameLayer* gjbgl, int p1, gd::vector<int> const* p2) {
		if (!Manager::getSharedInstance()->useCanonSpawn || !PlayLayer::get() || !PlayLayer::get()->m_level || PlayLayer::get()->m_level->m_levelID.value() != THE_DEEP_SEWERS) return CameraTriggerGameObject::triggerObject(gjbgl, p1, p2);
		for (int i = 0; i < this->m_groupCount; i++) {
			if (this->m_groups->at(i) != 900) continue;
			return log::info("[CAMERA] since we're using the canonical spawn location instead of the one on boomlings, disable trigger of ID {} targeting group {}", this->m_objectID, this->m_targetGroupID);
		}
		CameraTriggerGameObject::triggerObject(gjbgl, p1, p2);
	}
};

/*
so when colon told me:
	just updated the sewers
	those triggers are on group 900
it turns out he just added the group ID 900 without doing anything else to it
i ended up spending 5 hours scrambling to figure out wtf i did but went wrong
then checked the editor. and turns out i was running in circles for no reason
(none of the triggers were spawn triggered they just carry group id 900 lmao)
what a wonderful way for me to spend five hours spiraling with self-doubt! :D
might as well leave this code here for posterity in case things change again.
-- raydeeux
*/
/*
class $modify(MySpawnTriggerGameObject, SpawnTriggerGameObject) {
	void triggerObject(GJBaseGameLayer* gjbgl, int p1, gd::vector<int> const* p2){
		PlayLayer* pl = PlayLayer::get();
		if (!pl || gjbgl != pl || !pl->m_level || pl->m_level->m_levelID.value() != THE_DEEP_SEWERS || pl->m_levelSettings->m_spawnGroup != 81) return SpawnTriggerGameObject::triggerObject(gjbgl, p1, p2);
		log::info("this->m_objectID: {}", this->m_objectID);
		log::info("this->m_targetGroupID: {}", this->m_targetGroupID);
		if (this->m_targetGroupID == 900) return log::info("should've sent player to canonical start position, skipping trigger");
		SpawnTriggerGameObject::triggerObject(gjbgl, p1, p2);
	}
};
*/