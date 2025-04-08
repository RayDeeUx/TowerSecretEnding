// original file by alphalaneous written for colon's april fool's video
// adapted by raydeeux to abuse more const T& references >:D

#include "CreditsLayer.hpp"
#include "UserCell.hpp"
#include "Manager.hpp"

CreditsLayer* CreditsLayer::create() {
	auto* ret = new CreditsLayer();
	if (ret->init("Special thanks!")) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

void CreditsLayer::customSetup() {
	geode::ScrollLayer* scrollLayer = geode::ScrollLayer::create({356, 220});
	scrollLayer->m_contentLayer->setLayout(
		ColumnLayout::create()
			->setAxisReverse(true)
			->setAxisAlignment(AxisAlignment::End)
			->setAutoGrowAxis(220.f)
			->setGap(.0f)
	);

	bool isEven = false;

	for (const UserData& user : Manager::getSharedInstance()->users) {
		scrollLayer->m_contentLayer->addChild(UserCell::create(user, isEven));
		isEven = !isEven;
	}

	scrollLayer->m_contentLayer->updateLayout();
	scrollLayer->scrollToTop();

	m_listLayer->addChild(scrollLayer);
}

void CreditsLayer::showLayer(const bool instant) {
	GJDropDownLayer::showLayer(instant);
	this->removeFromParentAndCleanup(false);
	CCScene::get()->addChild(this);

	this->setID("CreditsLayer"_spr);
	m_listLayer->setID("list-of-users-layer"_spr);
}