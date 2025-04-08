#include "Utils.hpp"

using namespace geode::cocos;

namespace Utils {
	template<class T> T getSetting(const std::string_view setting) { return Mod::get()->getSettingValue<T>(setting); }

	bool getBool(const std::string_view setting) { return getSetting<bool>(setting); }
	
	int64_t getInt(const std::string_view setting) { return getSetting<int64_t>(setting); }
	
	double getDouble(const std::string_view setting) { return getSetting<double>(setting); }

	std::string getString(const std::string_view setting) { return getSetting<std::string>(setting); }

	ccColor3B getColor(const std::string_view setting) { return getSetting<ccColor3B>(setting); }

	ccColor4B getColorAlpha(const std::string_view setting) { return getSetting<ccColor4B>(setting); }

	bool modEnabled() { return getBool("enabled"); }
	
	bool isModLoaded(const std::string& modID) { return Loader::get()->isModLoaded(modID); }

	Mod* getMod(const std::string& modID) { return Loader::get()->getLoadedMod(modID); }

	std::string getModVersion(const Mod* mod) { return mod->getVersion().toNonVString(); }

	void logErrorCustomFormat(const std::string_view reason, const int robsID, const int colonsID) {
		log::info("———— ERROR HAPPENED ————");
		log::info("- the {} was invalid", reason);
		log::info("- RobTop's ID: {}", robsID);
		log::info("- Colon's ID: {}", colonsID);
		log::info("calling original function instead");
		log::info("——— END ERROR REPORT ———");
	}

	DialogLayer* showDialouge() {
		CCArray* dialougeObjects = CCArray::create();

		DialogObject* rattledashZero = DialogObject::create(HIS_NAME, "<cr>Well,<d020> well,<d020> well<d020>...</c>", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashZero->setTag(0);
		dialougeObjects->addObject(rattledashZero);

		DialogObject* rattledashOne = DialogObject::create(HIS_NAME, "I suppose you were <cl>stronger</c> than I thought.", 3, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashOne->setTag(1);
		dialougeObjects->addObject(rattledashOne);

		DialogObject* rattledashTwo = DialogObject::create(HIS_NAME, "<cj>I guess I'm nothing more than a pile of bones after all...</c>", 6, .5f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashTwo->setTag(2);
		dialougeObjects->addObject(rattledashTwo);

		DialogObject* rattledashThree = DialogObject::create(HIS_NAME, "But you haven't seen the last of me.", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashThree->setTag(3);
		dialougeObjects->addObject(rattledashThree);

		DialogObject* rattledashFour = DialogObject::create(HIS_NAME, "<cl>I WILL be back.</c>", 5, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashFour->setTag(4);
		dialougeObjects->addObject(rattledashFour);

		DialogObject* rattledashFive = DialogObject::create(HIS_NAME, "<cl>And the next time we cross paths...</c>", 5, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashFive->setTag(5);
		dialougeObjects->addObject(rattledashFive);

		DialogObject* rattledashSix = DialogObject::create(HIS_NAME, "<s300><cr>I'LL KICK YOUR ASS</c></s>", 8, 1.25f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashSix->setTag(6);
		dialougeObjects->addObject(rattledashSix);

		DialogObject* rattledashSeven = DialogObject::create(HIS_NAME, ".<d025>.<d025>.<d025>", 3, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashSeven->setTag(7);
		dialougeObjects->addObject(rattledashSeven);

		DialogObject* rattledashEight = DialogObject::create(HIS_NAME, "Perhaps another time.", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashEight->setTag(8);
		dialougeObjects->addObject(rattledashEight);

		return DialogLayer::createWithObjects(dialougeObjects, 4);
	}

}