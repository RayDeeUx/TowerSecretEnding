#pragma once
#include "UserCell.hpp"

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	bool calledAlready = false;

	const std::unordered_map<int, int> robtopToColon = {
		{5001, 116926133},
		{5002, 116926138},
		{5003, 116926140},
		{5004, 116926955},
	};

	const std::vector<UserData> users = {
		// username, iconID, color1, color2, glow, glowEnabled, userType, contributions
		{"Colon", 60, 18, 10, 10, true, UserType::Colon,
			{"Gameplay for <cy>The Secret Ending</c>", "Door highlighting in The Tower"}
		},
		{"16lord", 37, 12, 23, 12, true, UserType::Creator,
			{"Decoration and effects in <ca>The Foolish Tower</c>", "Intro cutscene in <cf>The Lost Cellar</c>", "Decoration in <co>The Sneaky Hollow</c>"}
		},
		{"galofuf", 110, 21, 11, 40, true, UserType::Creator,
			{"Cutscenes in <cg>The Deep Sewers</c>", "Decoration in <co>The Sneaky Hollow</c>"}
		},
		{"tricipital", 389, 10, 3, 42, true, UserType::Creator,
			{"Decoration in <cg>The Deep Sewers</c>"}
		},
		{"connot", 39, 41, 42, 3, true, UserType::Creator,
			{"Decoration in <co>The Sneaky Hollow</c>"}
		},
		{"Subwoofer", 394, 16, 83, 48, true, UserType::Creator,
			{"Decoration in <co>The Sneaky Hollow</c>"}
		},
		{"Mudstep", 30, 12, 3, 3, true, UserType::Musician,
			{"Custom music in <co>The Sneaky Hollow</c>"}
		},
		{"RayDeeUx", 104, 21, 3, 3, true, UserType::Modder,
			{"Translating <cy>The Secret Ending</c> into the Geode mod format", "Bug squasher (within reason)"}
		},
		{"acaruso", 12, 12, 17, 17, false, UserType::Modder,
			{"Downloading audio assets in the background"}
		},
		{"CJLink", 2, 12, 2, 15, true, UserType::Modder,
			{"Downloading levels in the background", "Opening online levels from outside of LevelInfoLayer"}
		},
		{"Alphalaneous", 452, 97, 42, 72, true, UserType::Modder,
			{"This \"Special thanks!\" list you're reading right now"}
		},
	};

	bool useCanonSpawn = false;

	bool colonModeEnabled = false;

	std::vector<int> completedLevels = {};

	static Manager* getSharedInstance() {
		if (!instance) {
			instance = new Manager();
		}
		return instance;
	}

};