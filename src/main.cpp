// original file by colon for his april fool's video
// adapted by raydeeux further because haha yes

#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

#include "CreditsLayer.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

class $modify(TowerButEpic, LevelAreaInnerLayer) {
	bool init(bool returningFromTowerLevel) {
		if (!LevelAreaInnerLayer::init(returningFromTowerLevel)) return false;
		Manager* manager = Manager::getSharedInstance();
		if (!manager->colonMode) return true;

		const int doorIndex = manager->doorToShow;
		if (doorIndex < 1) return true;

		CCNode* mainLayer = this->getChildByID("main-node");
		CCNode* doorLayer = mainLayer->getChildByID("main-menu");

		auto* door = doorLayer->getChildByType<CCMenuItemSpriteExtra>(doorIndex - 1);
		door->setSprite(CCSprite::create("towerDoorSpecial.png"_spr));
		door->setUserObject("current-door"_spr, CCBool::create(true));

		CCParticleSystemQuad* particles = GameToolbox::particleFromString("30a-1a2.2a0.48a8a90a180a29a0a11a0a0a0a0a0a0a0a3a1a0a0a0.607843a0a0.0196078a0a0a0a0.5a0a2a1a0a0a0.839216a0a0.0705882a0a0a0a0.3a0a0.54a0a0.57a0a40a0a6a0a-38a17a1a2a1a0a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", nullptr, false);
		particles->setPosition(door->getPositionX(), door->getPositionY() - 8);
		doorLayer->addChild(particles);
		particles->setScale(0.5f);

		return true;
	}

	void onInfo(cocos2d::CCObject* sender) {
		if (!Manager::getSharedInstance()->colonMode) return LevelAreaInnerLayer::onInfo(sender);
		CreditsLayer::create()->showLayer(false);
	}
};

class $modify(EndLevelLayer) {
	void customSetup() {
		EndLevelLayer::customSetup();
		if (!m_playLayer || !m_playLayer->m_level || !m_playLayer->m_level->getUserObject("colon-variant"_spr)) return;
		if (CCNode* lb = m_mainLayer->querySelector("button-menu > leaderboard-button")) lb->setVisible(false);
	}
};