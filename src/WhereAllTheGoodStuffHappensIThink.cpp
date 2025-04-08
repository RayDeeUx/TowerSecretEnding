// this file would've been named main.cpp but colon wanted me to include his source files too --raydeeux
#include <Geode/modify/CameraTriggerGameObject.hpp>
#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/DialogLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "Manager.hpp"
#include "Utils.hpp"
#include <ctime>

using namespace geode::prelude;

#define PLAYLAYER_LEVEL_ID m_level->m_levelID.value()
#define IS_LEVEL_COMPLETE(levelID) std::ranges::find(manager->completedLevels, levelID) != manager->completedLevels.end()
#define FORMATTED_DEBUG_LABEL fmt::format("\"If any of you ever come for my man, I'll break a ***** off like a KitKat bar.\"\n- Jane Wickline, 2025 [levelID: {}, pauseTimestamp - bombTimestamp: {}, lockedIn: {}]\n(canonPosition: {}, !colonVariant: {}, completed: {}, trackTime: {}, colonToggleUnlocked: {})", PlayLayer::get()->PLAYLAYER_LEVEL_ID, difftime(manager->pauseLayerTimestamp, manager->bombPickupTimestamp), manager->lockedIn, manager->useCanonSpawn, !PlayLayer::get()->m_level->getUserObject("colon-variant"_spr), IS_LEVEL_COMPLETE(PlayLayer::get()->PLAYLAYER_LEVEL_ID), manager->trackTime, manager->colonToggleUnlocked)
#define UPDATE_DEBUG_LABEL(source, originalCallback)\
	CCLabelBMFont* wicklineLabel = typeinfo_cast<CCLabelBMFont*>(source->getChildByID("jane-wickline-debug-label"_spr));\
	if (!Utils::getBool("debugMode") || !wicklineLabel) return originalCallback;\
	wicklineLabel->setString(FORMATTED_DEBUG_LABEL.c_str());\

class $modify(MyLevelAreaInnerLayer, LevelAreaInnerLayer) {
	void onColonToggle(CCObject* sender) {
		Manager* manager = Manager::getSharedInstance();
		if (!sender || !manager->completedVanillaTowerFloorOne) return; // click on it like a sane and sober human would, for chrissake!
		// just toggles the control. nothing crazy
		manager->colonMode = !manager->colonMode;
		Utils::highlightADoor(this, manager->colonMode);
	}
	bool init(bool returningFromTowerLevel) {
		if (!LevelAreaInnerLayer::init(returningFromTowerLevel)) return false;
		Manager* manager = Manager::getSharedInstance();
		if (!manager->completedVanillaTowerFloorOne) {
			if (!manager->shownHeadsUpDialog) {
				if (DialogLayer* headsUp = Utils::showHeadsUp(); headsUp) {
					this->addChild(headsUp);
					headsUp->animateInRandomSide();
					manager->shownHeadsUpDialog = true;
				}
			}
			return true;
		}

		if (!returningFromTowerLevel) {
			// download the levels! checking for nullptr from GLM *AND* level string length are most consistent solutions
			log::info("entering the tower from elsewhere");
			GameLevelManager* glm = GameLevelManager::get();
			if (!glm) return true;
			for (const auto&[robtopID, colonID] : manager->robtopToColon) {
				GJGameLevel* colonsLevel = glm->getSavedLevel(colonID);
				if (!colonsLevel || static_cast<std::string>(colonsLevel->m_levelString).length() < 2) glm->downloadLevel(colonID, false);
				colonsLevel = glm->getSavedLevel(colonID);
				if (colonsLevel && static_cast<std::string>(colonsLevel->m_levelString).length() > 2) log::info("colonsLevel {} with colonID {} was downloaded", colonsLevel, colonID);
			}
		}

		if (!manager->colonToggleUnlocked) return true;

		CCNode* backMenu = this->getChildByID("back-menu");
		if (!backMenu) return true;

		CCNode* backButton = backMenu->getChildByID("back-button");
		CCNode* vaultButton = backMenu->getChildByID("vault-button");
		if (!backButton || !vaultButton) return true;

		const std::string& checkmarkOne = !manager->colonMode ? "GJ_checkOff_001.png" : "GJ_checkOn_001.png";
		const std::string& checkmarkTwo = !manager->colonMode ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png";

		CCSprite* checkmarkOneSprite = CCSprite::createWithSpriteFrameName(checkmarkOne.c_str());
		CCSprite* checkmarkTwoSprite = CCSprite::createWithSpriteFrameName(checkmarkTwo.c_str());
		// scale the sprites before making the toggler button
		checkmarkOneSprite->setScale(.75f);
		checkmarkTwoSprite->setScale(.75f);

		CCMenuItemToggler* colonToggle = CCMenuItemToggler::create(
			checkmarkOneSprite, checkmarkTwoSprite, this,
			menu_selector(MyLevelAreaInnerLayer::onColonToggle)
		);
		colonToggle->setID("secret-ending-toggle"_spr);

		backMenu->addChild(colonToggle);
		colonToggle->setPosition({backButton->getPositionX(), vaultButton->getPositionY()});

		// apparently i need to modify the *sprites* to mimic the "fade-in" effect most of the other robtop buttons have.
		// pretty fuckin tedious but oh well --raydeeux
		checkmarkOneSprite->setOpacity(0);
		checkmarkTwoSprite->setOpacity(0);
		checkmarkOneSprite->runAction(CCFadeIn::create(.5f));
		checkmarkTwoSprite->runAction(CCFadeIn::create(.5f));

		return true;
	}
	void onDoor(CCObject* sender) {
		Manager* manager = Manager::getSharedInstance();
		const auto senderIsButton = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
		if (!sender || !manager->colonMode || !manager->completedVanillaTowerFloorOne || !senderIsButton) return LevelAreaInnerLayer::onDoor(sender);
		log::info("ok so the sender is in fact a button after all. proceed.");

		const int robtopsID = sender->getTag();
		if (robtopsID < 5001 || robtopsID > 5004) return LevelAreaInnerLayer::onDoor(sender); // because rob is going to add more tower levels. using tags here is ideal since they're 1:1 to rob's level IDs

		if (!senderIsButton->getUserObject("current-door"_spr)) return LevelAreaInnerLayer::onDoor(sender); // colon wants the levels to be played in order. do not report an error on this line; it is intended behavior

		const std::unordered_map<int, int>& robToColon = manager->robtopToColon;
		if (!robToColon.contains(robtopsID)) return LevelAreaInnerLayer::onDoor(sender);
		const int colonsID = robToColon.find(robtopsID)->second;

		GJGameLevel* colonsVersion = GameLevelManager::get()->getSavedLevel(colonsID);
		if (!colonsVersion) {
			Utils::logErrorCustomFormat("GJGameLevel", robtopsID, colonsID);
			return LevelAreaInnerLayer::onDoor(sender);
		}

		colonsVersion->setUserObject("colon-variant"_spr, CCBool::create(true));
		manager->useCanonSpawn = colonsID == THE_DEEP_SEWERS;

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

		Manager* manager = Manager::getSharedInstance();
		if (colonsID == THE_DEEP_SEWERS && !manager->useCanonSpawn) return GameManager::returnToLastScene(level);
		manager->useCanonSpawn = false;

		const bool sameSize = manager->completedLevels.size() == manager->correctCompletionOrder.size();
		bool shouldShowDialog = sameSize; // this is NOT a definitive declaration. still need to loop through vectors
		if (sameSize) {
			for (int i = 0; i < manager->completedLevels.size(); i++) {
				if (manager->completedLevels.at(i) != manager->correctCompletionOrder.at(i)) {
					shouldShowDialog = false;
					break;
				}
			}
			if (shouldShowDialog) {
				// reset everything
				manager->doorToShow = -1;
				manager->lockedIn = false;
				manager->trackTime = false;
				manager->colonMode = false;
				manager->completedLevels.clear();
				manager->colonToggleUnlocked = false;
				manager->completedAtLeastOnce = true; // let's be honest, endcredits for this mod are more important
			}
		}

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
		if (DialogLayer* rattledash = Utils::showRattledashsFinalWords(); shouldShowDialog && rattledash) {
			levelAreaInnerLayer->addChild(rattledash);
			rattledash->animateInRandomSide();
		}
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	void setupHasCompleted() {
		if (this->PLAYLAYER_LEVEL_ID != THE_DEEP_SEWERS || !this->m_levelSettings || this->m_levelSettings->m_spawnGroup != 79 || !this->m_level->getUserObject("colon-variant"_spr)) return PlayLayer::setupHasCompleted();
		this->m_levelSettings->m_spawnGroup = 81;
		PlayLayer::setupHasCompleted();
	}
	void startGame() {
		// this is really just for debug purposes.
		// the quote is just filler; ADHD go brrr.
		// --raydeeux
		if (!this->m_level || !this->getParent() || !Utils::getBool("debugMode")) return PlayLayer::startGame();

		Manager* manager = Manager::getSharedInstance();

		CCLabelBMFont* wicklineLabel = CCLabelBMFont::create(FORMATTED_DEBUG_LABEL.c_str(), "bigFont.fnt");
		wicklineLabel->setID("jane-wickline-debug-label"_spr);
		this->getParent()->addChild(wicklineLabel);
		wicklineLabel->limitLabelWidth(420.f, 2.f, 0.001f);
		wicklineLabel->setPosition(this->getParent()->getContentSize() / 2.f);

		PlayLayer::startGame();
	}
	void levelComplete() {
		if (!this->m_level || !this->getParent() || !this->m_level->getUserObject("colon-variant"_spr)) return PlayLayer::levelComplete();

		Manager* manager = Manager::getSharedInstance();
		const int levelID = this->PLAYLAYER_LEVEL_ID;
		if (const bool completed = IS_LEVEL_COMPLETE(levelID); !completed) manager->completedLevels.push_back(levelID);

		UPDATE_DEBUG_LABEL(this->getParent(), PlayLayer::levelComplete())

		manager->doorToShow += 1;

		PlayLayer::levelComplete();
	}
	void onQuit() {
		Manager* manager = Manager::getSharedInstance();
		manager->bombPickupTimestamp = std::time(nullptr);
		manager->pauseLayerTimestamp = std::time(nullptr);
		manager->trackTime = false;
		if (manager->colonToggleUnlocked) manager->lockedIn = true;
		PlayLayer::onQuit();
	}
};

class $modify(MyGJBaseGameLayer, GJBaseGameLayer) {
	virtual void spawnGroup(int groupBeingSpawned, bool p1, double p2, gd::vector<int> const& p3, int p4, int p5) {
		GJBaseGameLayer::spawnGroup(groupBeingSpawned, p1, p2, p3, p4, p5);
		if (PlayLayer::get() && groupBeingSpawned == 105 && this->m_level && this->PLAYLAYER_LEVEL_ID == 5003 && this->m_level->m_levelType == GJLevelType::Local) {
			Manager* manager = Manager::getSharedInstance();
			manager->bombPickupTimestamp = std::time(nullptr);
			manager->trackTime = true;
		}
	}
};

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		Manager* manager = Manager::getSharedInstance();
		if (!manager->trackTime || manager->colonToggleUnlocked) return PauseLayer::customSetup();

		manager->pauseLayerTimestamp = std::time(nullptr);
		const long secondsPassed = difftime(manager->pauseLayerTimestamp, manager->bombPickupTimestamp);

		manager->colonToggleUnlocked = secondsPassed < 3; // colon wants it to be lenient, i think 2 seconds is lenient enough tbh (original time window was 1 second) --raydeeux
		if (manager->colonToggleUnlocked) {
			manager->doorToShow = 1;
			manager->colonMode = true; // so the toggle is visually correct when entering LevelAreaInnerLayer
		}

		UPDATE_DEBUG_LABEL(CCScene::get(), PauseLayer::customSetup())
		PauseLayer::customSetup();
	}
	void onResume(CCObject* sender) {
		// if you unpause, you lose the colon toggle! yayyyyyy -raydeeux
		Manager* manager = Manager::getSharedInstance();
		if (manager->colonToggleUnlocked && !manager->lockedIn) {
			manager->colonToggleUnlocked = false; // so the toggle doesn't get added when entering LevelAreaInnerLayer
			manager->colonMode = false; // so the player has to redo the secret ending
			manager->doorToShow = -1;
		}
		UPDATE_DEBUG_LABEL(CCScene::get(), PauseLayer::onResume(sender))
		PauseLayer::onResume(sender);
	}
};

class $modify(MyEffectGameObject, EffectGameObject) {
	void triggerObject(GJBaseGameLayer* gjbgl, int p1, gd::vector<int> const* p2) {
		if (!Manager::getSharedInstance()->useCanonSpawn || !PlayLayer::get() || !PlayLayer::get()->m_level || PlayLayer::get()->PLAYLAYER_LEVEL_ID != THE_DEEP_SEWERS) return EffectGameObject::triggerObject(gjbgl, p1, p2);
		for (int i = 0; i < this->m_groupCount; i++) {
			if (this->m_groups->at(i) != 900) continue;
			// log::info("[EFFECT] since we're using the canonical spawn location instead of the one on boomlings, disable trigger of ID {} targeting group {}", this->m_objectID, this->m_targetGroupID);
			return;
		}
		EffectGameObject::triggerObject(gjbgl, p1, p2);
	}
};

class $modify(MyCameraTriggerGameObject, CameraTriggerGameObject) {
	void triggerObject(GJBaseGameLayer* gjbgl, int p1, gd::vector<int> const* p2) {
		if (!Manager::getSharedInstance()->useCanonSpawn || !PlayLayer::get() || !PlayLayer::get()->m_level || PlayLayer::get()->PLAYLAYER_LEVEL_ID != THE_DEEP_SEWERS) return CameraTriggerGameObject::triggerObject(gjbgl, p1, p2);
		for (int i = 0; i < this->m_groupCount; i++) {
			if (this->m_groups->at(i) != 900) continue;
			// log::info("[CAMERA] since we're using the canonical spawn location instead of the one on boomlings, disable trigger of ID {} targeting group {}", this->m_objectID, this->m_targetGroupID);
			return;
		}
		CameraTriggerGameObject::triggerObject(gjbgl, p1, p2);
	}
};

class $modify(MyDialogLayer, DialogLayer) {
	void displayDialogObject(DialogObject* dialogObject) {
		DialogLayer::displayDialogObject(dialogObject);
		const int tag = dialogObject->getTag();
		const bool isRattledash = this->getUserObject("rattledash"_spr);
		if (const std::array<std::string, 13>& dialogSprites = Manager::getSharedInstance()->listOfDialogSprites; isRattledash && tag < dialogSprites.size()) {
			this->m_characterSprite->initWithFile(dialogSprites.at(tag).c_str());
		}
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
		if (!pl || gjbgl != pl || !pl->m_level || pl->PLAYLAYER_LEVEL_ID != THE_DEEP_SEWERS || pl->m_levelSettings->m_spawnGroup != 81) return SpawnTriggerGameObject::triggerObject(gjbgl, p1, p2);
		log::info("this->m_objectID: {}", this->m_objectID);
		log::info("this->m_targetGroupID: {}", this->m_targetGroupID);
		if (this->m_targetGroupID == 900) return log::info("should've sent player to canonical start position, skipping trigger");
		SpawnTriggerGameObject::triggerObject(gjbgl, p1, p2);
	}
};
*/