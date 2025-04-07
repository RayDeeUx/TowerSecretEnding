#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

#include "CreditsLayer.hpp"
#include "UserCell.hpp"
#include "Manager.hpp"

using namespace geode::prelude;

class $modify(TowerButEpic, LevelAreaInnerLayer) {
	bool init(bool p0) {
		if (!LevelAreaInnerLayer::init(p0)) return false;

		const int doorIndex = Mod::get()->getSettingValue<int64_t>("doorToShowBETATESTINGLOL");
		if (doorIndex < 1) return true;

		CCNode* mainLayer = this->getChildByID("main-node");
		CCNode* doorLayer = mainLayer->getChildByID("main-menu");

		CCMenuItemSpriteExtra* door = doorLayer->getChildByType<CCMenuItemSpriteExtra>(doorIndex - 1);
		door->setSprite(CCSprite::create("towerDoorSpecial.png"_spr));

		CCParticleSystemQuad* particles = GameToolbox::particleFromString("30a-1a2.2a0.48a8a90a180a29a0a11a0a0a0a0a0a0a0a3a1a0a0a0.607843a0a0.0196078a0a0a0a0.5a0a2a1a0a0a0.839216a0a0.0705882a0a0a0a0.3a0a0.54a0a0.57a0a40a0a6a0a-38a17a1a2a1a0a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", nullptr, false);
		particles->setPosition(door->getPositionX(), door->getPositionY() - 8);
		doorLayer->addChild(particles);
		particles->setScale(0.5f);

		return true;
	}

	void onInfo(cocos2d::CCObject* sender) {
		if (!Manager::getSharedInstance()->colonModeEnabled) return LevelAreaInnerLayer::onInfo(sender);
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