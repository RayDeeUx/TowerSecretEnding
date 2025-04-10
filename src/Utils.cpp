#include "CustomDialogCallback.hpp"
#include "Manager.hpp"
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

	bool getSavedBool(const std::string_view savedValue) { return Mod::get()->getSavedValue<bool>(savedValue); }

	void logErrorCustomFormat(const std::string_view reason, const int robsID, const int colonsID) {
		log::info("———— ERROR HAPPENED ————");
		log::info("- the {} was invalid", reason);
		log::info("- RobTop's ID: {}", robsID);
		log::info("- Colon's ID: {}", colonsID);
		log::info("calling original function instead");
		log::info("——— END ERROR REPORT ———");
	}

	DialogLayer* showRattledashsFinalWords() {
		// also speaking of the dialouge layer i think i might just manually translate it into cocos code
		// using it in JSON format opens backdoor for wronguns to replace it with swears much more easily
		CCArray* dialougeObjects = CCArray::create();

		ADD_DUMMY_OBJECT_TO_TRICK_GD_INTO_REPLACING_SPRITES

		DialogObject* rattledashZero = DialogObject::create(HIS_NAME, "<cr>Well,<d020> well,<d020> well<d020>...</c>", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashZero->setTag(1); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashZero);

		DialogObject* rattledashOne = DialogObject::create(HIS_NAME, "I suppose you were <cl>stronger</c> than I thought.", 3, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashOne->setTag(2); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashOne);

		DialogObject* rattledashTwo = DialogObject::create(HIS_NAME, "<cj>I guess I'm nothing more than a pile of bones after all...</c>", 6, .5f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashTwo->setTag(3); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashTwo);

		DialogObject* rattledashThree = DialogObject::create(HIS_NAME, "But you haven't seen the last of me.", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashThree->setTag(4); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashThree);

		DialogObject* rattledashFour = DialogObject::create(HIS_NAME, "<cl>I WILL be back.</c>", 5, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashFour->setTag(5); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashFour);

		DialogObject* rattledashFive = DialogObject::create(HIS_NAME, "<cl>And the next time we cross paths...</c>", 5, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashFive->setTag(6); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashFive);

		DialogObject* rattledashSix = DialogObject::create(HIS_NAME, "<s300><cr>I'LL KICK YOUR ASS</c></s>", 8, 1.25f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashSix->setTag(7); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashSix);

		DialogObject* rattledashSeven = DialogObject::create(HIS_NAME, ".<d025>.<d025>.<d025>", 3, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashSeven->setTag(8); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashSeven);

		DialogObject* rattledashEight = DialogObject::create(HIS_NAME, "Perhaps another time.", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashEight->setTag(9); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashEight);

		CREATE_RATTLEDASH_WITH(dialougeObjects)
		ret->setUserObject("rattledashs-final-words"_spr, CCBool::create(true));

		// ok everything from this point on until the next inline comment is colon's code --raydeeux
		std::function<void()> customCallback = [=]() {
			Utils::showRattledashChest();
		};

		auto* del = new CustomDialogCallback();
		ret->addChild(del);
		del->autorelease();
		del->m_callback = customCallback;
		ret->m_delegate = del;
		// ok colon's code segment ends here

		return ret;
	}

	DialogLayer* showHeadsUp() {
		CCArray* dialougeObjects = CCArray::create();

		ADD_DUMMY_OBJECT_TO_TRICK_GD_INTO_REPLACING_SPRITES

		DialogObject* rattledashExtraOne = DialogObject::create(HIS_NAME, "\"Well met!\" \"Another time, then.\" \"Ah! Welcome!\"", 1, .5f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraOne->setTag(10); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraOne);

		DialogObject* rattledashExtraTwo = DialogObject::create(HIS_NAME, "Oh! You've quite startled me there.", 5, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraTwo->setTag(11); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraTwo);

		DialogObject* rattledashExtraThree = DialogObject::create(HIS_NAME, "Well, I'll be out of your way. Have fun exploring.", 3, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraThree->setTag(12); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraThree);

		CREATE_RATTLEDASH_WITH(dialougeObjects)

		return ret;
	}

	DialogLayer* showFailedDownload() {
		CCArray* dialougeObjects = CCArray::create();

		ADD_DUMMY_OBJECT_TO_TRICK_GD_INTO_REPLACING_SPRITES

		DialogObject* rattledashExtraFour = DialogObject::create(HIS_NAME, "Ah! Welcome!", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraFour->setTag(13); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraFour);

		DialogObject* rattledashExtraFive = DialogObject::create(HIS_NAME, "It looks like you're not ready for <cy>The Secret Ending</c> yet.", 5, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraFive->setTag(14); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraFive);

		DialogObject* rattledashExtraSix = DialogObject::create(HIS_NAME, "Another time, then.", 3, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraSix->setTag(15); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraSix);

		CREATE_RATTLEDASH_WITH(dialougeObjects)

		return ret;

	}

	DialogLayer* showAudioMissing() {
		CCArray* dialougeObjects = CCArray::create();

		ADD_DUMMY_OBJECT_TO_TRICK_GD_INTO_REPLACING_SPRITES

		DialogObject* rattledashExtraSeven = DialogObject::create(HIS_NAME, "Ah! Welcome!", 1, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraSeven->setTag(16); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraSeven);

		DialogObject* rattledashExtraEight = DialogObject::create(HIS_NAME, "<cy>The Secret Ending</c> is a little quiet right now.", 5, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraEight->setTag(17); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraEight);

		DialogObject* rattledashExtraNine = DialogObject::create(HIS_NAME, "Another time, then.", 3, 1.f, DEFAULT_DIALOUGE_OBJECT_SETTINGS);
		rattledashExtraNine->setTag(18); // see manager->listOfDialogSprites for more info
		dialougeObjects->addObject(rattledashExtraNine);

		CREATE_RATTLEDASH_WITH(dialougeObjects)

		return ret;

	}

	void highlightADoor(LevelAreaInnerLayer* scene, const bool isColonMode) {
		if (!scene) return;
		const int doorIndex = Manager::getSharedInstance()->doorToShow;
		if (doorIndex < 1) return;

		CCNode* mainLayer = scene->getChildByID("main-node");
		CCNode* doorLayer = mainLayer->getChildByID("main-menu");

		auto* door = doorLayer->getChildByType<CCMenuItemSpriteExtra>(doorIndex - 1);
		if (door->getTag() < 5001 || door->getTag() > 5004) return; // dont touch nodes that arent doors
		door->setSprite(isColonMode ? CCSprite::create("towerDoorSpecial.png"_spr) : CCSprite::createWithSpriteFrameName("towerDoor_open_001.png"));
		if (isColonMode) door->setUserObject("current-door"_spr, CCBool::create(true));

		if (!isColonMode) {
			if (CCNode* particle = doorLayer->getChildByID("current-door-particles"_spr)) particle->removeMeAndCleanup();
			return;
		}
		// if colon mode disabled don't add the particle

		CCParticleSystemQuad* particles = GameToolbox::particleFromString("30a-1a2.2a0.48a8a90a180a29a0a11a0a0a0a0a0a0a0a3a1a0a0a0.607843a0a0.0196078a0a0a0a0.5a0a2a1a0a0a0.839216a0a0.0705882a0a0a0a0.3a0a0.54a0a0.57a0a40a0a6a0a-38a17a1a2a1a0a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", nullptr, false);
		particles->setPosition(door->getPositionX(), door->getPositionY() - 8);
		doorLayer->addChild(particles);
		particles->setScale(0.5f);
		particles->setID("current-door-particles"_spr);
	}

	void showRattledashChest() {
		if (!Utils::getSavedBool("rattledashChest")) {
			// its fine if it just isnt there -- colon
			/*
			Manager* manager = Manager::getSharedInstance();
			if (!manager->shownCutForTime) {
				manager->shownCutForTime = true;
				return FLAlertLayer::create("Aww man...", "Unfortunately, Rattledash's chest was cut for time.\n<cy>Sorry about that!</c>", "OK")->show();
			}
			*/
			return;
		}
		// adapted from colon's JSON file + his code snippets --raydeeux
		CCArray* chestRewards = CCArray::create();

		chestRewards->addObject(GJRewardObject::createItemUnlock(UnlockType::Cube, 42)); // yeah sorry
		chestRewards->addObject(GJRewardObject::create(SpecialRewardItem::Orbs, 3000, 1));
		chestRewards->addObject(GJRewardObject::create(SpecialRewardItem::Diamonds, 200, 1));
		chestRewards->addObject(GJRewardObject::create(SpecialRewardItem::BonusKey, 3, 1));

		RewardUnlockLayer* rewardLayer = RewardUnlockLayer::create(static_cast<int>(GJRewardType::Key100Treasure), nullptr); // 8 == 100 key chest
		GJRewardItem* item = GJRewardItem::createWithObjects(GJRewardType::Key100Treasure, chestRewards);

		rewardLayer->setUserObject("rattledash-chest"_spr, CCBool::create(true));
		rewardLayer->show();
		rewardLayer->showCollectReward(item);

		rewardLayer->m_wrongLabel->setString("(this doesn't affect your stats lol)\n(also you're stuck with the creeper cube sorry)");
		rewardLayer->m_wrongLabel->setColor(ccColor3B{0, 255, 255});
		rewardLayer->m_wrongLabel->setPositionY(300.f);
		rewardLayer->m_wrongLabel->setVisible(true);
		rewardLayer->m_wrongLabel->setOpacity(0);

		rewardLayer->m_wrongLabel->runAction(CCFadeIn::create(2.f));
	}

	 void levelDownloadFailed() {
		Manager* manager = Manager::getSharedInstance();
		manager->downloadsFailed = true;
		if (manager->shownDownloadsFailed) return;
		DialogLayer* downloadFailedPopup = Utils::showFailedDownload();
		LevelAreaInnerLayer* lail = CCScene::get()->getChildByType<LevelAreaInnerLayer>(0);
		if (!downloadFailedPopup || !lail) return;
		lail->addChild(downloadFailedPopup);
		downloadFailedPopup->animateInRandomSide();
		downloadFailedPopup->displayNextObject();
		manager->shownDownloadsFailed = true;
		Utils::highlightADoor(lail, false);
		if (CCNode* toggler = lail->getChildByIDRecursive("secret-ending-toggle"_spr)) toggler->removeMeAndCleanup();
		if (CCNode* label = lail->getChildByIDRecursive("secret-ending-toggle-label"_spr)) label->removeMeAndCleanup();
		GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
	}


	bool checkForAllIn(const std::string& commaSeparatedListOfIDs, const bool isSong) {
		for (const std::string& assetID : utils::string::split(commaSeparatedListOfIDs, ",")) {
			const int integerID = utils::numFromString<int>(assetID).unwrapOr(-1585);
			if (integerID == -1585) continue;
			if (isSong) {
				if (!std::filesystem::exists(MusicDownloadManager::sharedState()->pathForSong(integerID))) {
					log::info("song ID {} is missing!", integerID);
					return false;
				}
			} else {
				if (!std::filesystem::exists(MusicDownloadManager::sharedState()->pathForSFX(integerID))) {
					log::info("SFX ID {} is missing!", integerID);
					return false;
				}
			}
		}
		return true;
	}

}