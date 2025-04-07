// original file by alphalaneous written for colon's april fool's video
// adapted by raydeeux to abuse more const T& references >:D

#pragma once

using namespace geode::prelude;

class CreditsLayer : public GJDropDownLayer {
public:
	static CreditsLayer* create();
	virtual void customSetup();
	virtual void showLayer(const bool instant);
};