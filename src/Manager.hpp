#pragma once
#include "UserCell.hpp"

#define DIALOUGE_SPRITE_ARRAY_SIZE 16

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	bool calledAlready = false;

	// cmon guys do i really need to explain this to you
	const std::map<int, int> robtopToColon = {
		{5001, 116926133},
		{5002, 116926138},
		{5003, 116926140},
		{5004, 116926955},
	};

	// this is a surprise tool that will be helpful later
	// sprite names for the DialogLayer::displayDialogObject hook
	const std::array<std::string, DIALOUGE_SPRITE_ARRAY_SIZE> listOfDialogSprites = {
		"dialogIcon_001.png"_spr, // THIS IS UNUSED. DO NOT CHANGE OR REMOVE THIS.
		"dialogIcon_001.png"_spr,
		"dialogIcon_003.png"_spr,
		"dialogIcon_006.png"_spr,
		"dialogIcon_001.png"_spr,
		"dialogIcon_005.png"_spr,
		"dialogIcon_005.png"_spr,
		"dialogIcon_008.png"_spr,
		"dialogIcon_003.png"_spr,
		"dialogIcon_001.png"_spr,
		"dialogIcon_001.png"_spr, // extra dialouge sprite for anyone who hasnt finished the tower yet
		"dialogIcon_005.png"_spr, // extra dialouge sprite for anyone who hasnt finished the tower yet
		"dialogIcon_003.png"_spr, // extra dialouge sprite for anyone who hasnt finished the tower yet
		"dialogIcon_005.png"_spr, // extra dialouge sprite for people who arnet downloading the levels properly
		"dialogIcon_005.png"_spr, // extra dialouge sprite for people who arnet downloading the levels properly
		"dialogIcon_005.png"_spr, // extra dialouge sprite for people who arnet downloading the levels properly
	};

	// for "special thanks" screen
	// easier to allocate memory for this info *once*
	// as opposed to each time the "special thanks" screen is opened
	const std::vector<UserData> users = {
		// username, iconID, color1, color2, glow, glowEnabled, userType, contributions
		/*
		{"Colon", 60, 18, 10, 10, true, UserType::Colon,
			{"Idea for <cy>The Secret Ending</c>"}
		},
		*/
		{"16lord", 37, 12, 23, 12, true, UserType::Creator,
			{"Decoration and power room VFX in <ca>The Foolish Tower</c>", "Intro cutscene in <cf>The Lost Cellar</c>", "Pre-boss cutscene in <co>The Sneaky Hollow</c>"}
		},
		{"galofuf", 110, 21, 11, 40, true, UserType::Creator,
			{"Cutscenes and pipe VFX in <cg>The Deep Sewers</c>", "Decoration and boss design in <co>The Sneaky Hollow</c>"}
		},
		{"tricipital", 389, 10, 3, 42, true, UserType::Creator,
			{"Decoration and polish in <cg>The Deep Sewers</c>"}
		},
		{"connot", 39, 41, 42, 3, true, UserType::Creator,
			{"Decoration and pre-chase cutscene in <co>The Sneaky Hollow</c>"}
		},
		{"Subwoofer", 394, 16, 83, 48, true, UserType::Creator,
			{"Decoration and monster chase sequence in <co>The Sneaky Hollow</c>"}
		},
		{"Mudstep", 30, 12, 3, 3, true, UserType::Musician,
			{"Custom boss theme in <co>The Sneaky Hollow</c> (ft. guitar by Elixterr)"}
		},
		/*
		{"RayDeeUx", 104, 21, 3, 3, true, UserType::Modder,
			{"Translating <cy>The Secret Ending</c> into the Geode mod format", "Bug squasher (within reason)"}
		},
		*/
		{"acaruso", 12, 12, 17, 17, false, UserType::Modder,
			{"Help with downloading audio assets in the background"}
		},
		{"dankmeme01", 1, 41, 16, 16, false, UserType::Modder,
			{"Additional help with SAFELY downloading levels in the background"}
		},
		{"CJLink", 2, 12, 2, 15, true, UserType::Modder,
			{"Help with opening online levels from outside of LevelInfoLayer"}
		},
		{"Alphalaneous", 452, 97, 42, 72, true, UserType::Modder,
			{"Designed this \"Special thanks!\" list you're reading right now", "Putting up with an \"RTFM\"-worthy question"}
		},
		{"hiimjasmine00", 41, 98, 16, 12, true, UserType::Modder,
			{"Level string size detection", "Crossplatform Geode SDK bindings support"}
		},
		{"Prevter", 457, 41, 63, 12, true, UserType::Modder,
			{"Crossplatform Geode SDK bindings support"}
		},
		{"Capeling", 1, 0, 3, 3, false, UserType::Modder,
			{"Putting up with an \"RTFM\"-worthy question"}
		},
		{"OmgRod", 165, 104, 100, 100, false, UserType::Modder,
			{"For their (albeit late) efforts <ca>o7</c>"}
		},
	};

	// for the deep sewers specifically
	bool useCanonSpawn = false;

	// determine if toggle should be added
	bool colonToggleUnlocked = false;
	// controls when this mod's behavior kicks in
	bool colonMode = false;

	// track completed levels during runtime to know when to show rattledash's final words
	// the correctCompletionOrder is to minimize cheating
	std::vector<int> completedLevels = {};
	const std::vector<int> correctCompletionOrder = {116926133, 116926138, 116926140, 116926955}; // in case people want to cheat for some reason

	// store timestamps between level ID 5003 bomb pickup at group 105
	// and pauselayer creation
	bool trackTime = false;
	bool lockedIn = false;
	std::time_t bombPickupTimestamp;
	std::time_t pauseLayerTimestamp;

	// integer for colon to highlight the correct door i think
	int doorToShow = -1;

	// control when to show alpha's "special thanks!" layer (while colonToggleUnlocked is false)
	bool completedAtLeastOnce = false;

	// check if someone has unlocked + completed all tower levels
	bool completedVanillaTowerFloorOne = false;
	// control if someone has already seen the "you havent unlocked and completed all tower levels yet" dialog
	bool shownHeadsUpDialog = false;

	// because user objects are not 100% accurate
	bool isFromColonsTower = false;

	// check if downloads were correct
	bool downloadsFailed = false;
	// only show the dialog once
	bool shownDownloadsFailed = false;

	// track first time entering the tower
	bool firstTimeEntering = true;

	// only show Cut for Time popup once
	bool shownCutForTime = false;

	static Manager* getSharedInstance() {
		if (!instance) instance = new Manager();
		return instance;
	}

};