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

}