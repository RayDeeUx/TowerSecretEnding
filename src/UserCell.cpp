// original file by alphalaneous written for colon's april fool's video
// adapted by raydeeux to abuse more const T& references >:D

#include <Geode/utils/web.hpp>
#include "UserCell.hpp"

UserCell* UserCell::create(const UserData& userData, const bool isEven) {
	auto* ret = new UserCell();

	if (ret->init(userData, isEven)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool UserCell::init(const UserData& userData, const bool isEven) {
	if (!CCLayerColor::initWithColor(isEven ? ccColor4B{161, 88, 44, 255} : ccColor4B{194, 114, 62, 255})) return false;
	m_userData = userData;

	this->setContentSize({356.f, 36.f});

	CCLabelBMFont* usernameLabel = CCLabelBMFont::create(userData.username.c_str(), "bigFont.fnt");
	usernameLabel->setAnchorPoint({.0f, .5f});
	usernameLabel->setPosition({60, getContentHeight() / 2.f + 1.f});
	usernameLabel->setScale(.7f);

	GameManager* gameManager = GameManager::get();

	SimplePlayer* player = SimplePlayer::create(userData.iconID);
	player->m_firstLayer->ignoreAnchorPointForPosition(true);
	player->setContentSize({30.5, 30.5});

	player->setColors(gameManager->colorForIdx(userData.iconColor1), gameManager->colorForIdx(userData.iconColor2));
	if (userData.showGlow) player->setGlowOutline(gameManager->colorForIdx(userData.glowColor));

	player->setAnchorPoint({.5f, .5f});
	player->setPosition({30.f, this->getContentHeight() / 2.f});
	player->setScale(0.75f);

	CCMenu* menu = CCMenu::create();
	menu->setContentSize({60.f, 30.f});
	menu->setAnchorPoint({.5f, .5f});
	menu->setPosition({305.f, this->getContentHeight() / 2.f});
	menu->ignoreAnchorPointForPosition(false);
	menu->setLayout(RowLayout::create()->setGap(10.f)->setAutoScale(true)->setDefaultScaleLimits(.0001f, 1.f)->setAxisReverse(true));

	CCMenuItemSpriteExtra* infoButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(UserCell::onUserCellInfo));
	menu->addChild(infoButton);
	menu->updateLayout();

	CCSprite* socialSprite;
	switch (m_userData.type) {
		default: socialSprite = CCSprite::createWithSpriteFrameName("geode.loader/homepage.png"); break;
		case UserType::Creator:
		case UserType::Musician: socialSprite = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png"); break;
		case UserType::Modder: socialSprite = CCSprite::createWithSpriteFrameName("geode.loader/github.png"); break;
	}

	CCMenuItemSpriteExtra* socialButton = CCMenuItemSpriteExtra::create(socialSprite, this, menu_selector(UserCell::onSocial));
	menu->addChild(socialButton);
	menu->updateLayout();

	CCLayerColor* divider = CCLayerColor::create({0, 0, 0, 127});
	divider->setContentSize({356.f, 0.5f});
	divider->setAnchorPoint({0.f, 0.f});

	this->addChild(usernameLabel);
	this->addChild(player);
	this->addChild(divider);
	this->addChild(menu);

	menu->setID(fmt::format("thanks-menu-{}"_spr, userData.username));
	infoButton->setID(fmt::format("thanks-info-{}"_spr, userData.username));
	socialButton->setID(fmt::format("thanks-social-{}"_spr, userData.username));
	usernameLabel->setID(fmt::format("thanks-username-{}"_spr, userData.username));
	player->setID(fmt::format("thanks-player-{}"_spr, userData.username));
	divider->setID(fmt::format("thanks-divider-{}"_spr, userData.username));
	this->setID(fmt::format("thanks-usercell-{}"_spr, userData.username));

	return true;
}

void UserCell::onUserCellInfo(CCObject* sender) {
	if (!sender) return;
	std::string title = "Person";
	switch (m_userData.type) {
		default: break;
		case UserType::Creator: title = "<cy>Creator</c>"; break;
		case UserType::Musician: title = "<cj>Musician</c>"; break;
		case UserType::Modder: title = "<cd>Modder</c>"; break;
	}
	std::string contributions;
	for (const std::string& contribution : m_userData.contributions) contributions = contributions.append(fmt::format("\n- {}", contribution));
	return FLAlertLayer::create(m_userData.username.c_str(), title.append(contributions), "Close")->show();
}

void UserCell::onSocial(CCObject* sender) {
	if (sender) return web::openLinkInBrowser(m_userData.socialURL);
}