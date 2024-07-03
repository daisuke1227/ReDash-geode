#include "RDButton.hpp"
#include "RDDailyButton.hpp"
#include "hooks/GauntletSelectLayer.hpp"
#include "hooks/LevelInfoLayer.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

class $modify(CrazyLayer, MenuLayer) {
	static void onModify(auto& self) {
        self.setHookPriority("MenuLayer::init", INT_MIN/2 + 1); // making sure its run before pages api
    }

	bool init() {
		if (!MenuLayer::init()) return false;
		
		auto loader = Loader::get();
		auto mod = Mod::get();

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

			CCArrayExt<CCNode*> children = closeMenu->getChildren();
			for (auto& child : children) {
				if (child->getID() != "close-button") {
					child->setVisible(false);
				}
			}
		}

		// MAIN MENU CHANGES (MIGHT BE BREAKING SOME STUFF)

		this->getChildByID("main-title")->setVisible(false);

		this->getChildByID("more-games-menu")->setVisible(false);
		this->getChildByID("social-media-menu")->setVisible(false); // might make RobTop logo visible later
		this->getChildByID("main-menu")->setVisible(false); // hehehehe >:D

		auto bottomMenu = this->getChildByID("bottom-menu");
		bottomMenu->setPosition(ccp(540.f, 190.f));
		bottomMenu->setScale(0.75f);
		bottomMenu->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAutoScale(true)->setGap(0.f)->setCrossAxisOverflow(true));
		bottomMenu->setContentHeight(305.f);
		bottomMenu->updateLayout();
		bottomMenu->setZOrder(1);
		
		auto rightMenu = this->getChildByID("right-side-menu");
		rightMenu->setPosition(ccp(213.f, 25.f));
		rightMenu->setScale(0.75f);
		rightMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setAxisReverse(true));
		rightMenu->setContentWidth(505.f);
		rightMenu->updateLayout();
		rightMenu->getChildByID("daily-chest-button")->setZOrder(4);

		auto questBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("RD_quests.png"_spr), this, menu_selector(CreatorLayer::onChallenge));
		rightMenu->addChild(questBtn, 3);

		auto mapPacksBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("RD_mappacks.png"_spr), this, menu_selector(CreatorLayer::onMapPacks));
		rightMenu->addChild(mapPacksBtn, 1);

		this->getChildByID("player-username")->setPositionY(this->getChildByID("player-username")->getPositionY() - 75.f);
		this->getChildByID("player-username")->setPositionX(540.f);
		this->getChildByID("player-username")->setScale(this->getChildByID("player-username")->getScale() - 0.1f);
		this->getChildByID("profile-menu")->setPosition(ccp(575.5f, 35.f));
		this->getChildByID("profile-menu")->setScale(0.75f);
		this->getChildByID("profile-menu")->setZOrder(1);		

		// NEW STUFF YAYY :D

		// background for the menu on the right (the one with the Geode and settings buttons)

		CCScale9Sprite* bottomMenuBG = CCScale9Sprite::create("square02b_001.png");
		bottomMenuBG->setColor({0,0,0});
		bottomMenuBG->setOpacity(100);
		bottomMenuBG->setPosition(bottomMenu->getPosition());
		bottomMenuBG->setPositionY(CCDirector::get()->getWinSize().height / 2.f);
		bottomMenuBG->setContentSize({54.f, 302.f});
		addChild(bottomMenuBG);

		// background for the account username thing

		// if (!Mod::get()->getSettingValue<bool>("remove-account-stats")) { // then make the account stats (obviously)
		// 	CCMenu* accountUsernameMenu = CCMenu::create();
		// 	accountUsernameMenu->setScale(0.5f);
		// 	accountUsernameMenu->setPosition(200.f, 302.f);
		// 	accountUsernameMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setAutoScale(false)->setCrossAxisOverflow(false)->setGap(27.f));
		// 	//accountUsernameMenu->setContentWidth(570.f);
		// 	accountUsernameMenu->setContentHeight(53.f);

		// 	SimplePlayer* playerNode = SimplePlayer::create(GameManager::get()->getPlayerFrame());
		// 	playerNode->setColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor()));
		// 	playerNode->setSecondColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor2()));
		// 	playerNode->setScale(1.25f);
		// 	accountUsernameMenu->addChild(playerNode);

		// 	CCLabelBMFont* usernameLabel = CCLabelBMFont::create(GJAccountManager::get()->m_username.c_str(), "goldFont.fnt");
		// 	accountUsernameMenu->addChild(usernameLabel);

		// 	addChild(accountUsernameMenu, 1);
		// 	accountUsernameMenu->updateLayout();

		// 	CCScale9Sprite* accUsrBG = CCScale9Sprite::create("square02b_001.png");
		// 	accUsrBG->setScale(0.5f);
		// 	accUsrBG->setColor({0,0,0});
		// 	accUsrBG->setOpacity(100);
		// 	accUsrBG->setPosition(accountUsernameMenu->getPosition());
		// 	accUsrBG->setContentSize({accountUsernameMenu->getContentWidth() + 55.f, accountUsernameMenu->getContentHeight() + 5.f});
		// 	addChild(accUsrBG);
		// }

		// CCMenu* timedLevels = CCMenu::create();
		// timedLevels->setID("timed-levels-menu"_spr);
		// timedLevels->setLayout(ColumnLayout::create()->setGap(10.f));
		// timedLevels->setScale(0.7f);
		// timedLevels->setPosition({490.f, 200.f});

		// timedLevels->addChild(CCMenuItemSpriteExtra::create(CircleButtonSprite::createWithSpriteFrameName("gj_dailyCrown_001.png"), this, menu_selector(CreatorLayer::onDailyLevel)));
		// timedLevels->addChild(CCMenuItemSpriteExtra::create(CircleButtonSprite::createWithSpriteFrameName("gj_weeklyCrown_001.png"), this, menu_selector(CreatorLayer::onWeeklyLevel)));

		// addChild(timedLevels);

		// timedLevels->updateLayout();

		auto menu = CCMenu::create();
		menu->setID("redash-menu"_spr);
		menu->setPosition({ 0.f , 0.f });
		this->addChild(menu, 99);

		CCMenu* mainMenu = CCMenu::create();
		mainMenu->setID("main-menu"_spr);
		mainMenu->setContentSize({675.f, 100.f});
		mainMenu->ignoreAnchorPointForPosition(false);
		mainMenu->setPositionX(256.5f);
		mainMenu->setScale(0.75f);
		mainMenu->setPositionY(105.f);
		mainMenu->setLayout(
			RowLayout::create()
				->setGap(10.f)
				->setGrowCrossAxis(true)
				->setAutoScale(false)
		);

		mainMenu->addChild(RDButton::create(this, "Create", "You have\n[n] Levels", "RD_create.png"_spr, menu_selector(CreatorLayer::onMyLevels)));
		mainMenu->addChild(RDButton::create(this, "Saved", "You have\n[n] Saved\nLevels", "RD_saved.png"_spr, menu_selector(CreatorLayer::onSavedLevels)));
		mainMenu->addChild(RDButton::create(this, "Lists", "[n] new\nLists", "RD_lists.png"_spr, menu_selector(CreatorLayer::onTopLists)));
		mainMenu->addChild(RDButton::create(this, "Scores", "Global\n[n]", "RD_leaderboards.png"_spr, menu_selector(CreatorLayer::onLeaderboards)));
		mainMenu->addChild(RDButton::create(this, "Gauntlets", "[s]\nGauntlet\nAdded", "RD_gauntlets.png"_spr, menu_selector(CreatorLayer::onGauntlets)));
		mainMenu->addChild(RDButton::create(this, "Featured", "[n] new\nLevels", "RD_featured.png"_spr, menu_selector(CreatorLayer::onFeaturedLevels)));
		mainMenu->addChild(RDButton::create(this, "Paths", "Place\nholder!", "RD_paths_02.png"_spr,  menu_selector(CreatorLayer::onPaths)));
		mainMenu->addChild(RDButton::create(this, "Search", "Search\nFor levels\nOnline!", "RD_search.png"_spr, menu_selector(CreatorLayer::onOnlineLevels)));
		mainMenu->updateLayout();
		menu->addChild(mainMenu);

		auto dailiesMenu = CCMenu::create();
		dailiesMenu->setID("dailies-menu"_spr);
		dailiesMenu->setContentSize({ 675.f , 100.f });
		dailiesMenu->ignoreAnchorPointForPosition(false);
		dailiesMenu->setPosition({ 255.f , 206.25f });
		dailiesMenu->setScale(0.75f);

		dailiesMenu->addChild(RDDailyButton::create(false, { 25.f , 0.f }, { 230.f , 135.f }));
		dailiesMenu->addChild(RDDailyButton::create(true, { 425.f , 0.f }, { 230.f , 135.f }));
		menu->addChild(dailiesMenu);

		if (loader->isModLoaded("alphalaneous.pages_api")) {
			bottomMenu->setUserObject("orientation", CCInteger::create(0)); // VERTICAL
			bottomMenu->setUserObject("disable-pages", CCBool::create(true)); 
			// bottomMenu->setContentHeight(265.f);
			bottomMenu->setLayout(as<ColumnLayout*>(bottomMenu->getLayout())->setAutoScale(false));

			rightMenu->setUserObject("orientation", CCInteger::create(1)); // HORIZONTAL
    		rightMenu->setUserObject("disable-pages", CCBool::create(true)); 
			rightMenu->setLayout(as<RowLayout*>(rightMenu->getLayout())->setAutoScale(false));
		}

		bottomMenu->updateLayout();
		rightMenu->updateLayout();

		return true;
	}
};

class $modify(Balls, CCKeyboardDispatcher) {
	void xd(CCObject* sender) {
		CCScene* sc = CreatorLayer::scene();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

    bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool down, bool repeat) {
        if (key == enumKeyCodes::KEY_OEMMinus && down && this->m_bAltPressed)
            Balls::xd(nullptr);

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
    }
};