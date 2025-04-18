// original file by colon for his april fool's video
// adapted by raydeeux further because haha yes

#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/modify/AccountHelpLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "CreditsLayer.hpp"
#include "Manager.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

class $modify(TowerButEpic, LevelAreaInnerLayer) {
	bool init(bool returningFromTowerLevel) {
		if (!LevelAreaInnerLayer::init(returningFromTowerLevel)) return false;
		Manager* manager = Manager::getSharedInstance();
		if (!manager->colonMode || !manager->completedVanillaTowerFloorOne || manager->downloadsFailed) return true;

		Utils::highlightADoor(this, true); // yeah i moved the function over to Utils namespace lol --raydeeux

		return true;
	}

	void onInfo(cocos2d::CCObject* sender) {
		if (this->getParent()->getChildByID("CreditsLayer"_spr)) return;
		if (const Manager* manager = Manager::getSharedInstance(); manager->colonMode || manager->completedAtLeastOnce) return CreditsLayer::create()->showLayer(false);
		LevelAreaInnerLayer::onInfo(sender);
	}
};

class $modify(EndLevelLayer) {
	void customSetup() {
		EndLevelLayer::customSetup();
		if (!m_playLayer || !m_playLayer->m_level || !m_playLayer->m_level->getUserObject("colon-variant"_spr)) return;
		if (CCNode* lb = m_mainLayer->querySelector("button-menu > leaderboard-button")) lb->setVisible(false);
	}
};

// alright mom said it's my turn to commit to main.cpp
// --raydeeux

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		Manager* manager = Manager::getSharedInstance();
		GameStatsManager* gsm = GameStatsManager::get();
		if (manager->calledAlready || !gsm) return true;
		manager->calledAlready = true;
		manager->completedVanillaTowerFloorOne = gsm->hasCompletedMainLevel(THE_TOWER) && gsm->hasCompletedMainLevel(THE_SEWERS) && gsm->hasCompletedMainLevel(THE_CELLAR) && gsm->hasCompletedMainLevel(THE_SECRET_HOLLOW);
		return true;
	}
};

class $modify(AccountHelpLayer) {
	virtual void FLAlert_Clicked(FLAlertLayer* alert, bool btnTwo) {
		AccountHelpLayer::FLAlert_Clicked(alert, btnTwo);
		if (GameStatsManager* gsm = GameStatsManager::get(); gsm && btnTwo && alert->getTag() == 3) { // to check for unlinking account
			Manager::getSharedInstance()->completedVanillaTowerFloorOne = gsm->hasCompletedMainLevel(THE_TOWER) && gsm->hasCompletedMainLevel(THE_SEWERS) && gsm->hasCompletedMainLevel(THE_CELLAR) && gsm->hasCompletedMainLevel(THE_SECRET_HOLLOW);
		}
	}
};