#pragma once

using namespace geode::prelude;

#define DEFAULT_DIALOUGE_OBJECT_SETTINGS false, ccColor3B{255, 255, 255}
#define ERROR_DIALOUGE_OBJECT_SETTINGS true, ccColor3B{255, 255, 255}
#define HIS_NAME "Rattledash"
#define ADD_DUMMY_OBJECT_TO_TRICK_GD_INTO_REPLACING_SPRITES\
	DialogObject* rattledashDummy = DialogObject::create("FOOBARBAZ", "THIS IS A DUMMY LINE TO FORCE GD TO SKIP TO THE NEXT DIALOUGE OBJECT. YOU SHOULDN'T BE ABLE TO SEE THIS.", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);\
	rattledashDummy->setTag(768);\
	dialougeObjects->addObject(rattledashDummy);\

#define CREATE_RATTLEDASH_WITH(dialougeObjectMemory)\
	DialogLayer* ret = DialogLayer::createWithObjects(dialougeObjectMemory, 4);\
	ret->setUserObject("rattledash"_spr, CCBool::create(true));

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

	bool getSavedBool(const std::string_view savedValue);
	void logErrorCustomFormat(const std::string_view reason, const int robsID, const int colonsID);
	DialogLayer* showRattledashsFinalWords();
	DialogLayer* showHeadsUp();
	DialogLayer* showFailedDownload();
	DialogLayer* showAudioMissing();
	void highlightADoor(LevelAreaInnerLayer* scene, const bool isColonMode);
	void showRattledashChest();

	void levelDownloadFailed();

	bool checkForAllIn(const std::string& commaSeparatedListOfIDs, const bool isSong);
}