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

	void doDialouge() {
		CCArray* dialougeObjects = CCArray::create();
		dialougeObjects->addObject(DialogObject::create("Rattledash", "<cr>Well,<d020> well,<d020> well<d020>...</c>", 1, 1.f, false, ccColor3B{255, 255, 255}));
		dialougeObjects->addObject(DialogObject::create("Rattledash", "I suppose you were <cl>stronger</c> than I thought.", 3, 1.f, false, ccColor3B{255, 255, 255}));
		dialougeObjects->addObject(DialogObject::create("Rattledash", "<cj>I guess I'm nothing more than a pile of bones after all...</c>", 6, .5f, false, ccColor3B{255, 255, 255}));
		dialougeObjects->addObject(DialogObject::create("Rattledash", "But you haven't seen the last of me.", 1, 1.f, false, ccColor3B{255, 255, 255}));
		dialougeObjects->addObject(DialogObject::create("Rattledash", "<cl>I WILL be back.</c>", 5, 1.f, false, ccColor3B{255, 255, 255}));
		dialougeObjects->addObject(DialogObject::create("Rattledash", "<cl>And the next time we cross paths...</c>", 5, 1.f, false, ccColor3B{255, 255, 255}));
		dialougeObjects->addObject(DialogObject::create("Rattledash", "<s300><cr>I'LL KICK YOUR ASS</c></s>", 8, 1.25f, false, ccColor3B{255, 255, 255}));
		DialogLayer* dialougeLayer = DialogLayer::createWithObjects(dialougeObjects, 4);
		#ifndef GEODE_IS_X64
		dialougeLayer->addToMainScene();
		#endif
		dialougeLayer->animateIn(DialogAnimationType::FromRight);
	}

}