#pragma once

using namespace geode::prelude;

#define THE_DEEP_SEWERS 116926138
#define DEFAULT_DIALOUGE_OBJECT_SETTINGS false, ccColor3B{255, 255, 255}
#define HIS_NAME "Rattledash"
#define HIS_MAJESTY_THE_ROYAL_PAIN_IN_THE_BUTT "Scratch"

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
	DialogLayer* showRattledashsFinalWords();
	DialogLayer* showHeadsUp();
	void highlightADoor(LevelAreaInnerLayer* scene, const bool isColonMode);
}