// #include "ui/RDButton.hpp"
// #include "RDDailyNode.hpp"
#include "ui/RDStatsNode.hpp"
#include "ui/RDMainButton.hpp"

#include "hooks/onBackHooks.hpp"
#include "hooks/GameLevelManager.hpp"
#include "hooks/LayersHooks.hpp" // same hook index bruh

#include "settings/StatsSettingNode.hpp"

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/cocos.hpp>

#include <fmt/core.h>
#define MAX_SECRET_COINS 164
#define addCreatorButton(mod, id, selector, texture) \
	if (loader->isModLoaded(mod) && selector != nullptr) {\
		auto btn = CCMenuItemSpriteExtra::create(\
			CCSprite::createWithSpriteFrameName(texture),\
			this,\
			selector\
		);\
		btn->setID(id);\
		rightMenu->addChild(btn);\
	}

std::string getPathString(int n) {
	switch (n) {
		case 1: return "Fire";
		case 2: return "Ice";
		case 3: return "Poison";
		case 4: return "Shadow";
		case 5: return "Lava";
		case 6: return "Earth";
		case 7: return "Blood";
		case 8: return "Metal";
		case 9: return "Light";
		case 10: return "Souls";
		case 100: return "All";
		default: return "None";
	}
}

std::string abbreviateNumber(int n) {
	if (n < 1000) return std::to_string(n);
	if (n < 1000000) return fmt::format("{:.1f}k", n / 1000.f);
	if (n < 1000000000) return fmt::format("{:.1f}m", n / 1000000.f);
	return fmt::format("{:.1f}b", n / 1000000000.f);
}

class $modify(CrazyLayer, MenuLayer) {
	struct Fields {
		CCMenuItemSpriteExtra* m_questBtn;
	};

	static void onModify(auto& self) {
        auto res = self.setHookPriority("MenuLayer::init", INT_MIN/2 + 1); // making sure its run before pages api but after index developer points
    }

	void setupButtons() {
		auto loader = Loader::get();
		auto bottomMenu = this->getChildByID("bottom-menu");
		auto rightMenu = this->getChildByID("right-side-menu");

		if (loader->isModLoaded("geode.texture-loader") && loader->isModLoaded("undefined0.gdtweaks")) {
			if (loader->getLoadedMod("undefined0.gdtweaks")->getSettingValue<bool>("replace-more-games-w-texture")) {
				if (auto menu = this->getChildByID("more-games-menu")) {
					if (auto button = menu->getChildByID("geode.texture-loader/texture-loader-button")) {
						rightMenu->addChild(button);
					}
				}
			}
		}

		if (auto closeMenu = this->getChildByID("close-menu")) {
			if (!closeMenu->getChildByID("close-button")) {
				auto cls_spr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
				cls_spr->setScale(0.7);
				auto cls_btn = CCMenuItemSpriteExtra::create(
					cls_spr, 
					this, 
					menu_selector(MenuLayer::onQuit));
				cls_btn->setID("close-button");
				closeMenu->addChild(cls_btn);
			}

			if (loader->isModLoaded("weebify.restartbtn")) {
				if (auto restartBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(closeMenu->getChildByID("restart-button"))) {
					closeMenu->removeChild(restartBtn);
					bottomMenu->addChild(restartBtn);
					if (auto sprite = getChildOfType<CCSprite>(restartBtn, 0)) {
						restartBtn->setContentSize(restartBtn->getContentSize() * 1.5f);
						sprite->setScale(sprite->getScale() * 1.5f);
						sprite->setPosition(restartBtn->getContentSize() / 2);
					}
				}
				if (auto reloadBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(closeMenu->getChildByID("reload-button"))) {
					closeMenu->removeChild(reloadBtn);
					bottomMenu->addChild(reloadBtn);
					if (auto sprite = getChildOfType<CCSprite>(reloadBtn, 0)) {
						reloadBtn->setContentSize(reloadBtn->getContentSize() * 1.5f);
						sprite->setScale(sprite->getScale() * 1.5f);
						sprite->setPosition(reloadBtn->getContentSize() / 2);
					}
				}
			}
			auto foundBtn = false;
			if (loader->isModLoaded("smjs.gdintercept")) {
				if (auto node = this->getChildByIDRecursive("smjs.gdintercept/blame-overcharged-menu")) {
					CCMenuItemSpriteExtra* button = nullptr;
					if (typeinfo_cast<CCSprite*>(node)) button = as<CCMenuItemSpriteExtra*>(node->getParent());
					else if (typeinfo_cast<CCMenuItemSpriteExtra*>(node)) button = as<CCMenuItemSpriteExtra*>(node);

					if (button) {
						button->removeFromParent();
						bottomMenu->addChild(button);
						if (auto sprite = getChildOfType<CCSprite>(button, 0)) {
							button->setContentSize(button->getContentSize() * 1.5f);
							sprite->setScale(sprite->getScale() * 1.5f);
							sprite->setPosition(button->getContentSize() / 2);
						}
					}
				}
			}
		}

		addCreatorButton("cvolton.betterinfo", "cvolton.betterinfo/main-button", Variables::BISelector, "RD_betterinfo.png"_spr);
		addCreatorButton("xanii.super_expert", "super-expert-button", Variables::SupExSelector, "RD_superexpert.png"_spr);
		addCreatorButton("minemaker0430.gddp_integration", "demon-progression-button", Variables::GDDPSelector, Mod::get()->getSettingValue<bool>("alt-gddp-texture") ? "RD_gddp2.png"_spr : "RD_gddp.png"_spr);
		addCreatorButton("spaghettdev.gd-roulette", "spaghettdev.gd-roulette/roulette-button", Variables::RouletteSelector, "RD_roulette.png"_spr);

		#ifdef GEODE_IS_ANDROID
		if (loader->isModLoaded("geode.devtools")) {
			auto btn = CCMenuItemSpriteExtra::create(
				CircleButtonSprite::createWithSprite("RD_devtools.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::MediumAlt),
				this,
				menu_selector(MenuLayer::onMoreGames)
			);
			btn->setID("devtools-button");
			bottomMenu->addChild(btn);
		}
		#endif
	}

	void onHideMenu(CCObject* sender) {
		if (!as<CCMenuItemToggler*>(sender)->isOn()) {
			this->getChildByID("redash-menu"_spr)->getChildByID("main-menu"_spr)->setVisible(false);
			this->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->setVisible(false);
			// this->getChildByID("bottom-menu")->setVisible(false);
			// this->getChildByID("bottom-menu-bg"_spr)->setVisible(false);
			// this->getChildByID("right-side-menu")->setVisible(false);
			// profileMenu->setVisible(false);
			// this->getChildByID("close-menu")->setVisible(false);
			// playerUsername->setVisible(false);
		} else {
			this->getChildByID("redash-menu"_spr)->getChildByID("main-menu"_spr)->setVisible(true);
			this->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->setVisible(true);
			// this->getChildByID("bottom-menu")->setVisible(true);
			// this->getChildByID("bottom-menu-bg"_spr)->setVisible(true);
			// this->getChildByID("right-side-menu")->setVisible(true);
			// profileMenu->setVisible(true);
			// this->getChildByID("close-menu")->setVisible(true);
			// playerUsername->setVisible(true);
		}
	}

	bool init() {
		if (!MenuLayer::init()) return false;

		auto loader = Loader::get();
		auto mod = Mod::get();
		auto gsm = GameStatsManager::sharedState();
		auto glm = GameLevelManager::sharedState();
		auto gm = GameManager::sharedState();
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		
		if (Variables::WeeklyLeft < 1) {
			glm->getGJDailyLevelState(GJTimedLevelType::Weekly);
		}
		if (Variables::DailyLeft < 1) {
			glm->getGJDailyLevelState(GJTimedLevelType::Daily);
		}
		if (Variables::EventLeft < 1) {
			glm->getGJDailyLevelState(GJTimedLevelType::Event);
		}

		if (!Mod::get()->getSettingValue<bool>("hide-bottom-buttons-texts")) {
			if (GJAccountManager::get()->m_accountID == 0) {
				Variables::GlobalRank = -1;
			} else if (Variables::OldStarsCount != gsm->getStat("6")) {
				Variables::GlobalRank = 0;
				glm->getLeaderboardScores("leaderboard_global");
			}
		}

		CrazyLayer::setupButtons();

		// MAIN MENU CHANGES (MIGHT BE BREAKING SOME STUFF) - ninXout
		// no it isn't - Weebify
		if (loader->isModLoaded("sofabeddd.geometry_dash")) {
			if (this->getChildByID("sofabeddd.geometry_dash/main-title-menu")) {
				this->getChildByID("sofabeddd.geometry_dash/main-title-menu")->setVisible(false);
			}
		} else {
			this->getChildByID("main-title")->setVisible(false);
		}

		if (loader->isModLoaded("shineua.minecraft_splash")) {
			if (auto button = this->getChildByID("minecraft-splash")) {
				button->setVisible(false);
			}
		}

		this->getChildByID("more-games-menu")->setVisible(false);
		this->getChildByID("social-media-menu")->setVisible(false); // might make RobTop logo visible later // no
		this->getChildByID("main-menu")->setVisible(false); // hehehehe >:D // ????

		if (this->getChildByID("level-editor-hint")) this->getChildByID("level-editor-hint")->setVisible(false);
		if (this->getChildByID("character-select-hint")) this->getChildByID("character-select-hint")->setVisible(false);

		// controller glyphs
		if (this->getChildByID("play-gamepad-icon")) this->getChildByID("play-gamepad-icon")->setVisible(false);
		if (this->getChildByID("editor-gamepad-icon")) this->getChildByID("editor-gamepad-icon")->setVisible(false);
		if (this->getChildByID("icon-kit-gamepad-icon")) this->getChildByID("icon-kit-gamepad-icon")->setVisible(false);
		if (this->getChildByID("settings-gamepad-icon")) this->getChildByID("settings-gamepad-icon")->setVisible(false);
		if (this->getChildByID("mouse-gamepad-icon")) this->getChildByID("mouse-gamepad-icon")->setVisible(false);
		if (this->getChildByID("mouse-gamepad-label")) this->getChildByID("mouse-gamepad-label")->setVisible(false);
		if (this->getChildByID("click-gamepad-icon")) this->getChildByID("click-gamepad-icon")->setVisible(false);
		if (this->getChildByID("click-gamepad-label")) this->getChildByID("click-gamepad-label")->setVisible(false);

		auto bottomMenu = this->getChildByID("bottom-menu");
		bottomMenu->setScale(0.75f);
		bottomMenu->setLayout(
			ColumnLayout::create()
				->setAxisReverse(true)
				->setAutoScale(true)
				->setGap(0.f)
				->setCrossAxisOverflow(false)
		);
		bottomMenu->setPositionX(winSize.width - 29.f);
		bottomMenu->setContentWidth(60.f);
		if (GJAccountManager::get()->m_accountID == 0) {
			bottomMenu->setPositionY(winSize.height/2);
			bottomMenu->setContentHeight(385.f);
		} else {
			bottomMenu->setPositionY(winSize.height/2 + 30.f);
			bottomMenu->setContentHeight(305.f);
		}
		bottomMenu->updateLayout();
		bottomMenu->setZOrder(1);
		
		auto rightMenu = this->getChildByID("right-side-menu");
		rightMenu->setPosition(ccp(177.5f, 25.f));
		rightMenu->setScale(0.75f);
		rightMenu->setLayout(
			RowLayout::create()
				->setAxisAlignment(AxisAlignment::Start)
				->setAxisReverse(true)
				->setAutoScale(true)
				->setCrossAxisOverflow(false)
		);
		rightMenu->setContentWidth(425.f);
		rightMenu->setContentHeight(60.f);
		rightMenu->updateLayout();
		rightMenu->getChildByID("daily-chest-button")->setZOrder(4);

		auto questBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("RD_quests.png"_spr), this, menu_selector(CreatorLayer::onChallenge));
		questBtn->setID("quest-button"_spr);
		auto piVar2 = CCSprite::createWithSpriteFrameName("exMark_001.png");
		piVar2->setScale(0.55f);
		piVar2->setVisible(false);
		piVar2->setPosition(questBtn->getContentSize() - ccp(4,8));
		piVar2->setID("quest-notif"_spr);
		questBtn->addChild(piVar2, 1);
		m_fields->m_questBtn = questBtn;
		for (int i = 1; i <= 3; i++) {
			if (auto challenge = gsm->getChallenge(i)) {
				if (challenge->m_canClaim) {
					piVar2->setVisible(true);
					break;
				}
			}
		}
		rightMenu->addChild(questBtn, 3);

		auto mapPacksBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("RD_mappacks.png"_spr), this, menu_selector(CreatorLayer::onMapPacks));
		rightMenu->addChild(mapPacksBtn, 1);

		auto playerUsername = this->getChildByID("player-username");
		playerUsername->setScale(playerUsername->getScale() - 0.1f);
		playerUsername->setPositionX(bottomMenu->getPositionX());
		playerUsername->setPositionY(bottomMenu->getPositionY() - bottomMenu->getScaledContentHeight()/2 - playerUsername->getScaledContentHeight()/2 + 2.9f);
		if (playerUsername->getScaledContentWidth() > 54.f) {
			playerUsername->setScale(54.f / playerUsername->getContentWidth());
		}
		auto profileMenu = this->getChildByID("profile-menu");
		profileMenu->setScale(0.75f);
		profileMenu->setPositionX(playerUsername->getPositionX() + profileMenu->getScaledContentWidth()/2 - 20.625f);
		profileMenu->setPositionY(playerUsername->getPositionY() - playerUsername->getScaledContentHeight()/2 - profileMenu->getScaledContentHeight()/2 - 1.f);
		profileMenu->setZOrder(1);
		profileMenu->updateLayout();

		// NEW STUFF YAYY :D - ninXout
		// yay - Weebify

		// background for the menu on the right (the one with the Geode and settings buttons)

		CCScale9Sprite* bottomMenuBG = CCScale9Sprite::create("square02b_001.png");
		bottomMenuBG->setID("bottom-menu-bg"_spr);
		bottomMenuBG->setColor({0,0,0});
		bottomMenuBG->setOpacity(100);
		bottomMenuBG->setPosition(bottomMenu->getPosition());
		bottomMenuBG->setPositionY(CCDirector::get()->getWinSize().height / 2.f);
		bottomMenuBG->setContentSize({54.f, 302.f});
		addChild(bottomMenuBG);

		auto menu = CCMenu::create();
		menu->setID("redash-menu"_spr);
		menu->setPosition({ 0.f , 0.f });
		this->addChild(menu, 99);

		CCMenu* mainMenu = CCMenu::create();
		mainMenu->setID("main-menu"_spr);
		mainMenu->setContentSize({675.f, 100.f});
		mainMenu->ignoreAnchorPointForPosition(false);
		mainMenu->setScale(0.75f);
		mainMenu->setPositionX((winSize.width - 56.f - mainMenu->getScaledContentWidth())/2 + mainMenu->getScaledContentWidth()/2);
		mainMenu->setPositionY(winSize.height/2 - 55.f);
		mainMenu->setLayout(
			RowLayout::create()
				->setGap(10.f)
				->setGrowCrossAxis(true)
				->setAutoScale(false)
		);

		int activePath = gsm->m_activePath;
		int pathProgress = gsm->getStat(std::to_string(activePath).c_str());

		if (pathProgress > 1000) pathProgress = 1000;
		if (activePath == 0) {
			bool completedAll = true;
			for (int i = 1; i < 11; i++) {
				if (gsm->getStat(std::to_string(29 + i).c_str()) < 1000) {
					completedAll = false;
					break;
				}
			}
			if (completedAll) {
				activePath = 129;
				pathProgress = 1000;
			}
		}

		mainMenu->addChild(RDButton::create(this, "RD_createLabel.png"_spr, {"You have", fmt::format("{} Levels", abbreviateNumber(LocalLevelManager::get()->m_localLevels->count()))}, "RD_create.png"_spr, 0.95f, menu_selector(CreatorLayer::onMyLevels), "create-button"));
		mainMenu->addChild(RDButton::create(this, "RD_savedLabel.png"_spr, {"You have", fmt::format("{} Saved", abbreviateNumber(glm->getSavedLevels(false, 0)->count())), "Levels"}, "RD_saved.png"_spr, 0.95f, menu_selector(CreatorLayer::onSavedLevels), "saved-button"));
		mainMenu->addChild(RDButton::create(this, "RD_pathsLabel.png"_spr, {getPathString(activePath - 29), fmt::format("{}/1000", pathProgress)}, "RD_paths.png"_spr, 0.8f, menu_selector(CreatorLayer::onPaths), "paths-button"));
		mainMenu->addChild(RDButton::create(this, "RD_leaderboardsLabel.png"_spr, {"Global", fmt::format("#{}", Variables::GlobalRank)}, "RD_leaderboards.png"_spr, 0.85f, menu_selector(CreatorLayer::onLeaderboards), "leaderboards-button"));
		mainMenu->addChild(RDButton::create(this, "RD_gauntletsLabel.png"_spr, {"Forest", "Gauntlet", "Added"}, "RD_gauntlets.png"_spr, 1.f, menu_selector(CreatorLayer::onGauntlets), "gauntlets-button"));
		mainMenu->addChild(RDButton::create(this, "RD_featuredLabel.png"_spr, {"Play new", "Featured", "levels"}, "RD_featured.png"_spr, 0.95f, menu_selector(CreatorLayer::onFeaturedLevels), "featured-button"));
		mainMenu->addChild(RDButton::create(this, "RD_listsLabel.png"_spr, {"Play rated", "Lists"}, "RD_lists.png"_spr, 1.f, menu_selector(CreatorLayer::onTopLists), "lists-button"));
		mainMenu->addChild(RDButton::create(this, "RD_searchLabel.png"_spr, {"Search" , "For levels", "online"}, "RD_search.png"_spr, 0.9f, menu_selector(CreatorLayer::onOnlineLevels), "search-button"));
		
		mainMenu->updateLayout();
		menu->addChild(mainMenu);

		auto dailiesMenu = CCMenu::create();
		dailiesMenu->setID("dailies-menu"_spr);
		dailiesMenu->setContentSize({ 630.f , 135.f });
		dailiesMenu->ignoreAnchorPointForPosition(false);
		dailiesMenu->setPositionX(mainMenu->getPositionX());
		dailiesMenu->setPositionY(winSize.height/2 + 59.375f);
		dailiesMenu->setScale(0.75f);
		dailiesMenu->setLayout(
			RowLayout::create()
				->setGap(10.f)
				->setAutoScale(false)
		);

		if (Mod::get()->getSettingValue<bool>("preview-2.21")) {
			dailiesMenu->addChild(RDMainButton::create({ 25.f , 0.f }, { 230.f , 152.7f }, "main-levels-button", 600/(230*4.f)));
			dailiesMenu->addChild(RDDailyNode::create(0, { 230.f , 152.7f }, "daily-node", 600/(230*4.f)));
			dailiesMenu->addChild(RDDailyNode::create(1, { 230.f , 152.7f }, "weekly-node", 600/(230*4.f)));
			dailiesMenu->addChild(RDDailyNode::create(2, { 230.f , 152.7f }, "event-node", 600/(230*4.f)));
			mainMenu->setPositionY(mainMenu->getPositionY() + 12.5f);
		} else {
			if (Mod::get()->getSettingValue<bool>("main-levels-leftmost")) {
				dailiesMenu->addChild(RDMainButton::create({ 25.f , 0.f }, { 150.f , 135.f }, "main-levels-button", 1.f));
				dailiesMenu->addChild(RDDailyNode::create(0, { 230.f , 135.f }, "daily-node", 1.f));
			} else {
				dailiesMenu->addChild(RDDailyNode::create(0, { 230.f , 135.f }, "daily-node", 1.f));
				dailiesMenu->addChild(RDMainButton::create({ 265.f , 0.f }, { 150.f , 135.f }, "main-levels-button", 1.f));
			}
			dailiesMenu->addChild(RDDailyNode::create(1, { 230.f , 135.f }, "weekly-node", 1.f));
		}


		dailiesMenu->updateLayout();
		menu->addChild(dailiesMenu);

		if (winSize.width < 562.25f) {
			auto newWidth = winSize.width - 56;
			dailiesMenu->setScale(0.75f * newWidth / 506.25f);
			dailiesMenu->setPositionX(newWidth / 2);
			mainMenu->setScale(dailiesMenu->getScale());
			mainMenu->setPositionX(dailiesMenu->getPositionX());
			mainMenu->setPositionY(dailiesMenu->getPositionY() - dailiesMenu->getScaledContentHeight()/2 - 7.5f - mainMenu->getScaledContentHeight()/2);
		}

		auto statsMenu = CCMenu::create();
		statsMenu->setID("stats-menu"_spr);
		statsMenu->setContentSize({ winSize.width/0.6f , 25.f });
		statsMenu->ignoreAnchorPointForPosition(false);
		statsMenu->setAnchorPoint({ 0.f, 0.5f });
		statsMenu->setPosition({ 52.f , winSize.height - 6.25f });
		statsMenu->setScale(0.6f);
		statsMenu->setLayout(
			RowLayout::create()
				->setAxisAlignment(AxisAlignment::Start)
				->setGap(25.f)
				->setAutoScale(false)
		);

		std::map<std::string, std::pair<std::string, std::string>> stats = {
			{"GJ_starsIcon_001.png", {fmt::format("{}", gsm->getStat("6")), "stars-stats"}},
			{"GJ_moonsIcon_001.png", {fmt::format("{}", gsm->getStat("28")), "moons-stats"}},
			{"GJ_coinsIcon_001.png", {fmt::format("{}/{}", gsm->getStat("8"), MAX_SECRET_COINS), "secret-coins-stats"}},
			{"GJ_coinsIcon2_001.png", {fmt::format("{}", gsm->getStat("12")), "user-coins-stats"}},
			{"GJ_demonIcon_001.png", {fmt::format("{}", gsm->getStat("5")), "demons-stats"}},
			{"GJ_diamondsIcon_001.png", {fmt::format("{}", gsm->getStat("13")), "diamonds-stats"}},
			{"currencyDiamondIcon_001.png", {fmt::format("{}", gsm->getStat("29")), "diamond-diamonds-stats"}},
			{"currencyOrbIcon_001.png", {fmt::format("{}", gsm->getStat("14")), "orbs-stats"}}
		};
		auto settings = as<StatsSettingValue*>(mod->getSetting("stats-nodes-selection"));
		for (auto& stat : settings->getStatsArray()) {
			auto statStr = stat.as_string();
			if (stats.find(statStr) != stats.end()) {
				auto& [texture, text] = stats[statStr];
				statsMenu->addChild(RDStatsNode::create(statStr, texture, text));
			}
		}

		statsMenu->updateLayout();
		menu->addChild(statsMenu);

		auto menuButUnder = CCMenu::create();
		menuButUnder->setID("bottom-menu"_spr);
		menuButUnder->setAnchorPoint({ 1.f, 0.5f });
		menuButUnder->setContentSize({ 150.f, 48.25f });
		menuButUnder->setPosition({ winSize.width - 124.f, 25.f }); // ...
		auto bottomX = winSize.width - 124.f;
		menuButUnder->setScale(0.75f);
		menuButUnder->setLayout(
			RowLayout::create()
				->setAxisReverse(true)
				->setAxisAlignment(AxisAlignment::End)
				->setAutoScale(false)
				->setGap(0.f)
		);

		if (!Mod::get()->getSettingValue<bool>("disable-creator-button")) {
			auto creatorSpr = CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png");
			creatorSpr->setScale(0.9f);
			auto creatorBtn = CCMenuItemSpriteExtra::create(creatorSpr, this, menu_selector(MenuLayer::onCreator));
			creatorBtn->setID("creator-layer-button");
			menuButUnder->addChild(creatorBtn);
			bottomX -= creatorBtn->getScaledContentWidth()*menuButUnder->getScale();
		}
		auto treasureSpr = CCSprite::createWithSpriteFrameName("secretDoorBtn_open_001.png");
		treasureSpr->setScale(1.25f);
		auto treasureBtn = CCMenuItemSpriteExtra::create(treasureSpr, this, menu_selector(CreatorLayer::onTreasureRoom));
		treasureBtn->setID("treasure-room-button");
		menuButUnder->addChild(treasureBtn);
		bottomX -= treasureBtn->getScaledContentWidth()*menuButUnder->getScale();

		menuButUnder->updateLayout();
		rightMenu->setContentWidth((bottomX - rightMenu->getPositionX() + rightMenu->getScaledContentWidth()/2) / rightMenu->getScale());
		rightMenu->setPositionX(bottomX - rightMenu->getScaledContentWidth()/2);

		menu->addChild(menuButUnder);

		auto topMenu = CCMenu::create();
		topMenu->setID("top-menu"_spr);
		topMenu->setAnchorPoint({ 1.f, 0.5f});
		topMenu->setContentSize({ 100.f, 48.25f });
		topMenu->setPosition({ winSize.width - 79.f, winSize.height - 20.f });
		auto topX = winSize.width - 79.f;
		topMenu->setScale(0.75f);
		topMenu->setLayout(
			RowLayout::create()
				->setAxisReverse(true)
				->setAxisAlignment(AxisAlignment::End)
				->setAutoScale(false)
				->setGap(10.f)
		);

		auto garageSpr = CCSprite::createWithSpriteFrameName("garageRope_001.png");
		auto garageButton = CCMenuItemSpriteExtra::create(garageSpr, this, menu_selector(MenuLayer::onGarage));
		garageButton->setID("garage-rope");
		garageButton->setScale(4/3);
		topMenu->addChild(garageButton);
		topX -= 7.5;
		topX -= garageButton->getScaledContentWidth()*topMenu->getScale();

		auto vaultBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName(gsm->getStat("13") > 49 ? "GJ_lock_open_001.png" : "GJ_lockGray_001.png"), this, menu_selector(CreatorLayer::onSecretVault));
		vaultBtn->setID("secret-vault-button");
		topMenu->addChild(vaultBtn);
		topX -= 7.5;
		topX -= vaultBtn->getScaledContentWidth()*topMenu->getScale();

		topMenu->updateLayout();
		auto statsX = statsMenu->getPositionX();
		for (auto& statNode : CCArrayExt<RDStatsNode*>(statsMenu->getChildren())) {
			statsX += (statNode->getScaledContentWidth() + 25.f)*statsMenu->getScale();
		}
		statsX -= 25.f*statsMenu->getScale();
		if (statsX > topX) {
			statsMenu->setScale(0.6f * (topX - statsMenu->getPositionX()) / (statsX - statsMenu->getPositionX()));
		}

		garageButton->m_animationType = MenuAnimationType::Move;
		garageButton->m_startPosition = garageSpr->getPosition();
		garageButton->m_offset = ccp(0.f, -8.f);

		menu->addChild(topMenu);

		auto hideBtnMenu = CCMenu::create();
		hideBtnMenu->setID("hide-button-menu"_spr);
		hideBtnMenu->setAnchorPoint({ 1.f, 0.5f });
		hideBtnMenu->setContentSize({ 48.25f, 48.25f });
		hideBtnMenu->setPosition({ winSize.width - 129.f, 0.f });
		hideBtnMenu->setScale(0.75f);
		this->addChild(hideBtnMenu, 100);

		auto spr1 = CCSprite::create("RD_hideButton_01.png"_spr);
		auto spr2 = CCSprite::create("RD_hideButton_02.png"_spr);
		// spr2->setOpacity(50);
		auto hideToggler = CCMenuItemToggler::create(spr1, spr2, this, menu_selector(CrazyLayer::onHideMenu));
		hideToggler->setID("hide-button");
		hideToggler->setPosition({ hideBtnMenu->getContentWidth() / 2.f, hideBtnMenu->getContentHeight() / 2.f });
		hideBtnMenu->addChild(hideToggler);

		auto pagesMod = loader->getLoadedMod("alphalaneous.pages_api");
		if (pagesMod->getSettingValue<bool>("menulayer-bottom-menu")) {
			bottomMenu->setScale(0.975);
			bottomMenu->setContentHeight(bottomMenu->getContentHeight() - 90.f);
			bottomMenu->setUserObject("orientation", CCInteger::create(0)); // VERTICAL
			bottomMenu->setUserObject("element-count", CCInteger::create(5));	
		}

		if (pagesMod->getSettingValue<bool>("menulayer-right-menu")) {
			rightMenu->setLayout(as<RowLayout*>(rightMenu->getLayout())->setAutoScale(false));
			rightMenu->setScale(0.95);
			rightMenu->setContentWidth(rightMenu->getContentWidth() - 75.f);
			rightMenu->setUserObject("orientation", CCInteger::create(1)); // HORIZONTAL
			rightMenu->setUserObject("element-count", CCInteger::create((int)((rightMenu->getContentWidth() + 75.f) / 60.f)));
		}

		bottomMenu->updateLayout();
		rightMenu->updateLayout();

		return true;
	}
};

#include <Geode/modify/ChallengesPage.hpp>
class $modify(ChallengesPage) {
	void claimItem(ChallengeNode* node, GJChallengeItem* item, cocos2d::CCPoint point) {
		ChallengesPage::claimItem(node, item, point);

		if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
			auto piVar2 = as<CrazyLayer*>(layer)->m_fields->m_questBtn->getChildByID("quest-notif"_spr);
			bool claimable = false;
			for (int i = 1; i <= 3; i++) {
				if (auto challenge = GameStatsManager::sharedState()->getChallenge(i)) {
					if (challenge->m_canClaim) {
						claimable = true;
						break;
					}
				}
			}
			piVar2->setVisible(claimable);
		}
	}
};