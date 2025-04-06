#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "Manager.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		GameLevelManager* glm = GameLevelManager::get();
		if (!glm) return true;
		for (const auto&[robtopID, colonID] : Manager::getSharedInstance()->robtopToColon) {
			glm->downloadLevel(colonID, false);
			GJGameLevel* colonsVersion = glm->getSavedLevel(colonID);
			if (!colonsVersion) {
				Utils::logErrorCustomFormat("GJGameLevel (from forloop in MenuLayer init)", robtopID, colonID);
				continue;
			}
			LevelInfoLayer* colonLevelLayer = LevelInfoLayer::create(colonsVersion, false);
			if (!colonLevelLayer || !colonLevelLayer->m_songWidget || !colonLevelLayer->m_songWidget->m_downloadBtn) {
				Utils::logErrorCustomFormat("LevelInfoLayer (from forloop in MenuLayer init)", robtopID, colonID);
				continue;
			}
			colonLevelLayer->m_songWidget->m_downloadBtn->activate();
			log::info("releasing dummy LevelInfoLayer and GJGameLevel for colonLevelLayer ID {}", colonID);
			colonLevelLayer->release();
			colonsVersion->release();
			log::info("release successful");
		}
		return true;
	}
};

class $modify(MyLevelAreaInnerLayer, LevelAreaInnerLayer) {
	bool init(bool returning) {
		if (!LevelAreaInnerLayer::init(returning)) return false;
		if (returning) log::info("returning from a tower level");
		else log::info("entering the tower from elsewhere");
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
		CCScene* levelAreaInnerLayer = LevelAreaInnerLayer::scene(true);
		CCTransitionFade* transition = CCTransitionFade::create(0.5f, levelAreaInnerLayer);
		if (!transition) {
			Utils::logErrorCustomFormat("CCTransitionFade", robtopsID, colonsID);
			return GameManager::returnToLastScene(level);
		}
		CCDirector::sharedDirector()->replaceScene(transition); // safely free PlayLayer to avoid bugs
		log::info("pushing scene to LevelAreaInnerLayer");
		GameManager::fadeInMenuMusic(); // mimic vanilla behavior
	}
};