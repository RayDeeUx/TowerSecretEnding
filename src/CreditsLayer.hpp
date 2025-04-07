#pragma once

using namespace geode::prelude;

class CreditsLayer : public GJDropDownLayer {
public:
	static CreditsLayer* create();
	virtual void customSetup();
	virtual void showLayer(const bool instant);
};