// original file by alphalaneous written for colon's april fool's video
// adapted by raydeeux to abuse more const T& references >:D

#pragma once

using namespace geode::prelude;

enum class UserType {
	Colon,
	Creator,
	Musician,
	Modder
};

struct UserData {
	std::string username;
	int iconID;
	int iconColor1;
	int iconColor2;
	int glowColor;
	bool showGlow;
	UserType type;
	std::vector<std::string> contributions;
};

class UserCell : public CCLayerColor {
public:
	static UserCell* create(const UserData& userData, const bool isEven);
	bool init(const UserData& userData, const bool isEven);
	void onUserCellInfo(CCObject* sender);
	UserData m_userData;
};