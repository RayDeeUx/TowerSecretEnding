#pragma once

using namespace geode::prelude;

#define THE_DEEP_SEWERS 116926138

namespace Utils {
	template<class T> T getSetting(const std::string_view setting);
	bool getBool(const std::string_view setting);
	int64_t getInt(const std::string_view setting);
	double getDouble(const std::string_view setting);
	std::string getString(const std::string_view setting);
	ccColor3B getColor(const std::string_view setting);
	ccColor4B getColorAlpha(const std::string_view setting);
	bool modEnabled();
	
	bool isModLoaded(const std::string& modID);
	Mod* getMod(const std::string& modID);
	std::string getModVersion(const Mod* mod);

	void logErrorCustomFormat(const std::string_view reason, const int robsID, const int colonsID);
}