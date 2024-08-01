#include "RDDailyNode.hpp"

int getBonusDiamonds(GJGameLevel* level) {
    if (level->m_demon.value()) return 20;
    if (level->m_stars.value() == 1) return 0;
    return level->getAverageDifficulty();
}

bool RDDailyNode::init(int levelType, CCSize size, std::string id, float scale) {
    if (!CCNode::init()) return false;
    // 0: daily
    // 1: weekly
    // 2: event
    auto GLM = GameLevelManager::get();

    std::vector<const char*> bgVctr = {"GJ_square01.png", "GJ_square05.png", "GJ_square05.png"};
    auto background = CCScale9Sprite::create(bgVctr[levelType]);
    if (levelType == 2) {
        background->setColor({ 200, 50, 255 });
    }
    background->setContentSize(size * scale);
    background->setPosition({ size.width/2*scale, size.height/2*scale });
    background->setID("background");
    this->addChild(background, -1);

    auto node = CCNode::create();
    node->setContentSize(size);
    node->setScale(scale);
    node->setID("main-node");
    this->addChild(node, 0);
    m_mainNode = node;

    std::vector<const char*> crownVctr = {"gj_dailyCrown_001.png", "gj_weeklyCrown_001.png", "RD_eventCrown_001.png"_spr};
    auto crownSprite = CCSprite::createWithSpriteFrameName(crownVctr[levelType]);
    crownSprite->setScale(0.75f);
    crownSprite->setPosition({ size.width/2, size.height + 8.f });
    crownSprite->setID("crown-sprite");
    node->addChild(crownSprite, 1);

    std::vector<const char*> titleArr = {"dailyLevelLabel_001.png", "weeklyLevelLabel_001.png", "eventLevelLabel_001.png"};
    auto titleSprite = CCSprite::createWithSpriteFrameName(titleArr[levelType]);
    titleSprite->setScale(0.7f);
    titleSprite->setPosition({ size.width/2, size.height - 22.5f });
    titleSprite->setID("title-sprite");
    node->addChild(titleSprite, 1);

    auto innerBG = CCScale9Sprite::create("square02b_001.png");
    innerBG->setScale(0.5f);
    innerBG->setContentSize({ size.width*2 - 30.f, size.height / 1.16f });
    innerBG->setPosition({ size.width/2, size.height/2 });
    innerBG->setColor({ 0, 0, 0 });
    innerBG->setOpacity(50);
    innerBG->setID("inner-background");
    node->addChild(innerBG, 1);
    m_innerBG = innerBG;

    auto menu = CCMenu::create();
    menu->setPosition({ 0.f, 0.f });
    menu->setContentSize(size);
    menu->setID("level-menu");
    node->addChild(menu, 2);
    m_menu = menu;

    auto loadingCircle = LoadingCircle::create();
    loadingCircle->setScale(0.75f);
    loadingCircle->setContentSize({ 0 , 0 });
    loadingCircle->setPosition({ size.width/2, size.height/2 });
    loadingCircle->setVisible(false);
    loadingCircle->m_sprite->setPosition({ 0 , 0 });
    loadingCircle->m_sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
    loadingCircle->setID("loading-circle");
    node->addChild(loadingCircle, 2);
    m_loadingCircle = loadingCircle;


    auto bonusBG = CCScale9Sprite::create("GJ_square02.png");
    bonusBG->setScale(0.5f);
    bonusBG->setContentSize({ size.width/2.5f, size.height/5.5f });
    bonusBG->setContentSize(bonusBG->getContentSize() * 2);
    bonusBG->setPosition({ size.width/5.f + 7.5f , size.height/11.f + 10.f });
    bonusBG->setColor({ 0, 0, 0 });
    bonusBG->setOpacity(50);
    bonusBG->setID("bonus-background");
    node->addChild(bonusBG, 1);

    auto bonusMenu = CCMenu::create();
    bonusMenu->setContentSize(bonusBG->getScaledContentSize());
    bonusMenu->setPosition(bonusBG->getPosition());
    std::vector<float> bonusOffset = {2.f, 5.f, 2.f};
    bonusMenu->setPositionX(bonusMenu->getPositionX() + bonusOffset[levelType]);
    bonusMenu->setID("bonus-menu");
    bonusMenu->setLayout(
        RowLayout::create()
        ->setGap(0.f)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setAutoScale(false)
    );
    node->addChild(bonusMenu, 2);
    m_bonusMenu = bonusMenu;

    auto safeMenu = CCMenu::create();
    safeMenu->setPosition({ 0.f, 0.f });
    safeMenu->setContentSize(size);
    safeMenu->setID("safe-button-menu");
    node->addChild(safeMenu, 2);

    auto safeButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_safeBtn_001.png"),
        this,
        menu_selector(RDDailyNode::onTheSafe)
    );
    safeButton->setScale(0.5f);
    // safeButton->setPosition(size - ccp(2, 2));
    safeButton->setPosition({ size.width/2.f, bonusBG->getPositionY() });
    safeButton->m_scaleMultiplier = 1.15f;
    safeButton->m_baseScale = 0.5f;
    safeButton->setID("safe-button");
    node->addChild(safeButton);
    safeMenu->addChild(safeButton);

    std::vector<int> timelyUnk = {GLM->m_dailyIDUnk, GLM->m_weeklyIDUnk, GLM->m_eventIDUnk};
    if (auto level = GLM->getSavedDailyLevel(timelyUnk[levelType])) {
        RDDailyNode::setupLevelMenu(level);
    } else {
        std::vector<int> downloadType = {-1, -2, -3};
        GLM->downloadLevel(downloadType[levelType], false);
        m_loadingCircle->setVisible(true);
        m_bonusMenu->setVisible(false);
    }
    
    std::vector<int> timelyLeft = {Variables::DailyLeft, Variables::WeeklyLeft, Variables::EventLeft};
    auto time = timelyLeft[levelType];
    if (time < 0) time = 0;
    auto timeLabel = CCLabelBMFont::create(GameToolbox::getTimeString(time, true).c_str(), Mod::get()->getSettingValue<bool>("use-pusab") ? "bigFont.fnt" : "gjFont16.fnt");
    timeLabel->setScale(0.55f);
    timeLabel->setAnchorPoint({ 1, 0.5f });
    timeLabel->setPosition({ innerBG->getPositionX() + innerBG->getScaledContentSize().width/2 , size.height/9 });
    timeLabel->setID("time-label");
    node->addChild(timeLabel, 1);
    m_timeLabel = timeLabel;

    std::vector<const char*> timeLabelVctr = {"New Daily in:", "New Weekly in:", "New Event in:"};
    auto timeLeftLabel = CCLabelBMFont::create(timeLabelVctr[levelType], "bigFont.fnt");
    timeLeftLabel->setScale(0.3f);
    timeLeftLabel->setAnchorPoint({ 1, 0.5f });
    timeLeftLabel->setPosition({ timeLabel->getPositionX(), timeLabel->getPositionY() + timeLabel->getScaledContentHeight()/2 + timeLeftLabel->getScaledContentHeight()/2});
    timeLeftLabel->setColor({ 200, 200, 200 });
    timeLeftLabel->setID("time-left-label");
    node->addChild(timeLeftLabel, 1);
    m_timeLeftLabel = timeLeftLabel;

    auto timerLoadingCircle = LoadingCircle::create();
    timerLoadingCircle->setScale(0.45f);
    timerLoadingCircle->setContentSize({ 0 , 0 });
    timerLoadingCircle->setPositionX(timeLeftLabel->getPositionX() - timeLeftLabel->getScaledContentWidth()/2);
    timerLoadingCircle->setPositionY((timeLeftLabel->getPositionY() + timeLabel->getPositionY())/2);
    timerLoadingCircle->setVisible(false);
    timerLoadingCircle->m_sprite->setPosition({ 0 , 0 });
    timerLoadingCircle->m_sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
    timerLoadingCircle->setID("timer-loading-circle");
    node->addChild(timerLoadingCircle, 2);
    m_timerLoadingCircle = timerLoadingCircle;

    if (time == 0) {
        timeLabel->setVisible(false);
        timeLeftLabel->setVisible(false);
        timerLoadingCircle->setVisible(true);
    }

    this->setContentSize(size * scale);
    this->setID(id);

    return true;
}

void RDDailyNode::onView(CCObject* sender) {
    auto sc = LevelInfoLayer::scene(m_currentLevel, false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
}

void RDDailyNode::onClaimReward(CCObject* sender) {
    auto reward = GameStatsManager::sharedState()->completedDailyLevel(m_currentLevel);
    auto viewButton = as<CCMenuItemSpriteExtra*>(m_menu->getChildByID("view-button"));
    auto skipButton = as<CCMenuItemSpriteExtra*>(m_menu->getChildByID("skip-button"));
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto GLM = GameLevelManager::get();
    std::vector<std::string> claimedTimely = {"claimed-daily", "claimed-weekly", "claimed-event"};
    Mod::get()->setSavedValue(claimedTimely[m_levelType], true);

    if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
        auto point = ccp(232.49, 200);
        if (auto menu = layer->getChildByID("redash-menu"_spr)) {
            if (auto dailiesMenu = menu->getChildByID("dailies-menu"_spr)) {
                if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(sender)) {
                    point.x = dailiesMenu->getPositionX() - dailiesMenu->getScaledContentSize().width/2 + this->getPositionX()*dailiesMenu->getScaleX() + button->getPositionX() - button->getScaledContentSize().width*1.5f;
                    point.y = dailiesMenu->getPositionY() - dailiesMenu->getScaledContentSize().height/2 + this->getPositionY()*dailiesMenu->getScaleY() + button->getPositionY() - button->getScaledContentSize().height/2;
                    
                }
            }
        }

        if (m_levelType == 1) {
            int diamonds = 0;
            int orbs = 0;
            int keySprite = 0;
            int key = 0;
            int shardsType = 0;
            int shards = 0;
            for (auto t : CCArrayExt<GJRewardObject*>(reward->m_rewardObjects)) {
                switch (as<int>(t->m_specialRewardItem)) {
                    case 7:
                        orbs = t->m_total;
                        break;
                    case 8:
                        diamonds = t->m_total;
                        break;
                    case 6:
                        keySprite = 9;
                        key = t->m_total;
                    // shards
                    case 1:
                        shardsType = 4;
                        shards = t->m_total;
                        break;
                    case 2:
                        shardsType = 5;
                        shards = t->m_total;
                        break;
                    case 3:
                        shardsType = 6;
                        shards = t->m_total;
                        break;
                    case 4:
                        shardsType = 7;
                        shards = t->m_total;
                        break;
                    case 5:
                        shardsType = 8;
                        shards = t->m_total;
                        break;
                    case 10:
                        shardsType = 10;
                        shards = t->m_total;
                        break;
                    case 11:
                        shardsType = 11;
                        shards = t->m_total;
                        break;
                    case 12:
                        shardsType = 12;
                        shards = t->m_total;
                        break;
                    case 13:
                        shardsType = 13;
                        shards = t->m_total;
                        break;
                    case 14:
                        shardsType = 14;
                        shards = t->m_total;
                        break;
                }
            }

            auto rewardLayer = CurrencyRewardLayer::create(orbs, 0, 0, diamonds, as<CurrencySpriteType>(keySprite), key, as<CurrencySpriteType>(shardsType), shards, point, as<CurrencyRewardType>(1), 0, 1);
            layer->addChild(rewardLayer, 1000);
            FMODAudioEngine::sharedEngine()->playEffect("gold01.ogg");
            FMODAudioEngine::sharedEngine()->playEffect("secretKey.ogg");
        } else {
            auto rewardLayer = CurrencyRewardLayer::create(0, 0, 0, getBonusDiamonds(m_currentLevel), as<CurrencySpriteType>(0), 0, as<CurrencySpriteType>(0), 0, point, as<CurrencyRewardType>(0), 0, 1);
            layer->addChild(rewardLayer, 1000);
            FMODAudioEngine::sharedEngine()->playEffect("gold02.ogg");
        }
    }

    viewButton->setScale(0.6f);
    viewButton->m_baseScale = 0.6f;
    viewButton->m_scaleMultiplier = 1.15f;
    viewButton->setPosition({ m_menu->getContentWidth()*5.5f/6.5f, m_menu->getContentHeight()/2.f });
    m_menu->getChildByID("claim-button")->removeFromParent();


    std::vector<int> timelyUnk = {GLM->m_dailyIDUnk, GLM->m_weeklyIDUnk, GLM->m_eventIDUnk};
    std::vector<int> timelyID = {GLM->m_dailyID, GLM->m_weeklyID, GLM->m_eventID};
    if (timelyUnk[m_levelType] < timelyID[m_levelType]) {
        skipButton->setVisible(true);
    } else {
        skipButton->setVisible(false);
    }
}

void RDDailyNode::onSkiplevel(CCObject* sender) {
    std::vector<const char*> timelyName = {"daily", "weekly", "event"};
    geode::createQuickPopup(
        "Skip level",
        fmt::format("There is a <cy>new</c> {} level available.\nSkip the current level and load the next?", timelyName[m_levelType]),
        "Cancel", "Skip",
        [this](auto, bool btn2) {
            if (btn2) {
                m_loadingCircle->setVisible(true);
                m_menu->removeAllChildren();
                m_bonusMenu->setVisible(false);
                GameLevelManager::get()->downloadLevel(-m_levelType - 1, false);
                std::vector<std::string> claimedTimely = {"claimed-daily", "claimed-weekly", "claimed-event"};
                Mod::get()->setSavedValue(claimedTimely[m_levelType], false);
            }
        }
    );
}

void RDDailyNode::onCreatorLabel(CCObject* sender) {
    ProfilePage::create(m_currentLevel->m_accountID.value(), false)->show();
}

void RDDailyNode::onReload(CCObject* sender) {
    m_loadingCircle->setVisible(true);
    m_menu->removeAllChildren();

    std::vector<int> timelyLeft = {Variables::DailyLeft, Variables::WeeklyLeft, Variables::EventLeft};
    if (timelyLeft[m_levelType] == 0) GameLevelManager::get()->getGJDailyLevelState(as<GJTimedLevelType>(m_levelType));
    GameLevelManager::get()->downloadLevel(-m_levelType - 1, false);
}

void RDDailyNode::onTheSafe(CCObject* sender) {
    auto search = GJSearchObject::create(as<SearchType>(m_levelType + 21));
    auto scene = LevelBrowserLayer::scene(search);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
}

void RDDailyNode::downloadLevelFailed() {
    m_loadingCircle->setVisible(false);
    m_menu->removeAllChildren();

    auto reloadButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"),
        this,
        menu_selector(RDDailyNode::onReload)
    );
    reloadButton->m_scaleMultiplier = 1.15f;
    reloadButton->setPosition({ m_menu->getContentWidth()*5.5f/6.5f, m_menu->getContentHeight()/2.f });
    reloadButton->setID("reload-button");
    m_menu->addChild(reloadButton);

    auto label = CCLabelBMFont::create("Something went wrong...", "goldFont.fnt");
    label->setScale(0.5f);
    label->setPositionX((m_innerBG->getPositionX() - m_innerBG->getScaledContentWidth()/2 + reloadButton->getPositionX() - reloadButton->getScaledContentWidth()/2)/2);
    label->setPositionY(reloadButton->getPositionY());
    label->setID("error-label");
    m_menu->addChild(label);
}

void RDDailyNode::setupBonusMenu(GJGameLevel* level) {
    m_bonusMenu->removeAllChildren();

    auto bonusSprite = m_levelType == 1 ? 
        CCSprite::createWithSpriteFrameName("chest_03_02_001.png") :
        CCSprite::createWithSpriteFrameName("GJ_bigDiamond_001.png");
    bonusSprite->setScale(m_levelType == 1 ? 0.16f : 0.4f);
    bonusSprite->setID("bonus-sprite");
    m_bonusMenu->addChild(bonusSprite);

    auto bonusLabel = CCLabelBMFont::create((m_levelType == 1 ? " / Bonus" : fmt::format("x{} Bonus", getBonusDiamonds(level)).c_str()), "bigFont.fnt");
    bonusLabel->setScale(0.4f);
    bonusLabel->setID("bonus-label");
    m_bonusMenu->addChild(bonusLabel);

    m_bonusMenu->updateLayout();
    m_bonusMenu->setVisible(true);
}

void RDDailyNode::setupLevelMenu(GJGameLevel* level) {
    auto GLM = GameLevelManager::get();

    m_menu->removeAllChildren();
    m_loadingCircle->setVisible(false);
    setupBonusMenu(level);

    m_currentLevel = level;
    int difficultyRating = 0;
    int featureRating = 0;
    if (level->m_demon.value()) {
        if (level->m_demonDifficulty == 0) difficultyRating += 2;
        difficultyRating = level->m_demonDifficulty + 4;
    } else {
        difficultyRating = level->getAverageDifficulty();
    }

    if (level->m_featured) featureRating += 1;
    featureRating += level->m_isEpic;

    auto viewButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png"),
        this,
        menu_selector(RDDailyNode::onView)
    );
    viewButton->setScale(0.6f);
    viewButton->m_baseScale = 0.6f;
    viewButton->m_scaleMultiplier = 1.15f;
    viewButton->setPosition({ m_menu->getContentWidth()*5.5f/6.5f, m_menu->getContentHeight()/2.f }); // dont ask...
    viewButton->setID("view-button");
    m_menu->addChild(viewButton, 10);

    auto skipButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"),
        this,
        menu_selector(RDDailyNode::onSkiplevel)
    );

    std::vector<int> timelyUnk = {GLM->m_dailyIDUnk, GLM->m_weeklyIDUnk, GLM->m_eventIDUnk};
    std::vector<int> timelyID = {GLM->m_dailyID, GLM->m_weeklyID, GLM->m_eventID};
    if (timelyUnk[m_levelType] < timelyID[m_levelType]) {
        skipButton->setVisible(true);
    } else {
        skipButton->setVisible(false);
    }
    skipButton->setScale(0.5f);
    skipButton->setPosition({ m_innerBG->getPositionX() - m_innerBG->getScaledContentSize().width/2 + 5.f, m_innerBG->getPositionY() + m_innerBG->getScaledContentSize().height/2 });
    skipButton->m_baseScale = 0.5f;
    skipButton->m_scaleMultiplier = 1.2f;
    skipButton->setID("skip-button");
    m_menu->addChild(skipButton, 10);
    m_skipButton = skipButton;

    if (level->m_normalPercent.value() == 100 && !GameStatsManager::sharedState()->hasCompletedDailyLevel(timelyUnk[m_levelType])) {
        auto claimButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_rewardBtn_001.png"),
            this,
            menu_selector(RDDailyNode::onClaimReward)
        );
        claimButton->setScale(0.6f);
        claimButton->m_baseScale = 0.6f;
        claimButton->m_scaleMultiplier = 1.15f;
        claimButton->setPosition(viewButton->getPosition() + ccp(10.f, 0));
        claimButton->setID("claim-button");
        m_menu->addChild(claimButton, 10);
        
        viewButton->setScale(0.2f);
        viewButton->m_baseScale = 0.2f;
        viewButton->setPosition(claimButton->getPosition() - ccp(claimButton->getScaledContentWidth()/2 + viewButton->getScaledContentWidth()/2 + 2.5f, 0));

        skipButton->setVisible(false);
    }


    auto difficultySprite = GJDifficultySprite::create(difficultyRating, GJDifficultyName::Short);
    difficultySprite->updateFeatureState(as<GJFeatureState>(featureRating));
    difficultySprite->setScale(0.8f);
    difficultySprite->setPosition({ m_menu->getContentWidth()/7.5f, viewButton->getPositionY() + 2.5f });
    difficultySprite->setID("difficulty-sprite");
    m_menu->addChild(difficultySprite, 9);

    auto menuSize = m_innerBG->getScaledContentSize();
    auto baseY = m_innerBG->getPositionY() - menuSize.height/2;

    auto maxX = viewButton->getPositionX() - viewButton->getScaledContentWidth()/2 - difficultySprite->getPositionX() - difficultySprite->getScaledContentWidth()/2 - 5.f;

    auto nameLabel = CCLabelBMFont::create(level->m_levelName.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.5f);
    if (nameLabel->getScaledContentWidth() > maxX) {
        nameLabel->setScale(maxX / nameLabel->getContentWidth());
    }
    nameLabel->setAnchorPoint({ 0, 0.5f });
    nameLabel->setPositionX(difficultySprite->getPositionX() + difficultySprite->getScaledContentWidth()/2 + 5.f);
    nameLabel->setPositionY(baseY + menuSize.height*3/4);
    nameLabel->setID("name-label");
    m_menu->addChild(nameLabel, 8);

    auto creatorLabel = CCLabelBMFont::create(fmt::format("by {}", level->m_creatorName).c_str(), "goldFont.fnt");
    creatorLabel->setScale(0.5f);
    if (creatorLabel->getScaledContentWidth() > maxX) {
        creatorLabel->setScale(maxX / creatorLabel->getContentWidth());
    }
    auto creatorButton = CCMenuItemSpriteExtra::create(
        creatorLabel,
        this,
        menu_selector(RDDailyNode::onCreatorLabel)
    );
    creatorButton->setPositionX(nameLabel->getPositionX() + creatorLabel->getScaledContentWidth()/2);
    creatorButton->setPositionY(baseY + menuSize.height/2);
    creatorButton->m_scaleMultiplier = 1.1f;
    creatorButton->setID("creator-button");
    m_menu->addChild(creatorButton, 8);

    auto songName = "[Failed to load song name]";
    if (auto song = MusicDownloadManager::sharedState()->getSongInfoObject(level->m_songID)) {
        songName = song->m_songName.c_str();
    }
    auto songLabel = CCLabelBMFont::create(songName, "bigFont.fnt");
    songLabel->setScale(0.35f);
    if (songLabel->getScaledContentWidth() > maxX) {
        songLabel->setScale(maxX / songLabel->getContentWidth());
    }
    songLabel->setAnchorPoint({ 0, 0.5f });
    songLabel->setColor({ 250, 110, 245 });
    songLabel->setPositionX(nameLabel->getPositionX());
    songLabel->setPositionY(baseY + menuSize.height/4);
    songLabel->setID("song-label");
    m_menu->addChild(songLabel, 8);

    auto starsLabel = CCLabelBMFont::create(std::to_string(level->m_stars).c_str(), "bigFont.fnt");
    starsLabel->setScale(0.32f);
    starsLabel->setPositionX(difficultySprite->getPositionX() - 5.f);
    starsLabel->setPositionY(baseY + menuSize.height/6);
    starsLabel->setID("stars-label");
    m_menu->addChild(starsLabel, 7);

    auto starSprite = CCSprite::createWithSpriteFrameName("star_small01_001.png");
    starSprite->setScale(0.8f);
    starSprite->setPositionX(starsLabel->getPositionX() + starsLabel->getScaledContentWidth()/2 + 5.f);
    starSprite->setPositionY(baseY + menuSize.height/6);
    starSprite->setID("star-sprite");
    m_menu->addChild(starSprite, 8);

    // pretty much copied from the level thumbnails mod code lmao
    if (auto image = typeinfo_cast<CCImage*>(Variables::ThumbnailsDict->objectForKey(fmt::format("thumbnail-{}", level->m_levelID.value())))) {
        downloadThumbnailFinished(image);
    } else {
        m_listener.bind([this] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                if (!res->ok()) {
                    downloadThumbnailFail();
                } else {
                    auto data = res->data();
                    auto thread = std::thread([this, data]() {
                        auto image = Ref(new CCImage());
                        image->autorelease();
                        image->initWithImageData(const_cast<uint8_t*>(data.data()), data.size());
                        Variables::ThumbnailsDict->setObject(image, fmt::format("thumbnail-{}", m_currentLevel->m_levelID.value()));

                        geode::Loader::get()->queueInMainThread([this, image](){
                            downloadThumbnailFinished(image);
                        });
                    });
                    thread.detach();
                }
            }
        });

        auto req = web::WebRequest();
        m_listener.setFilter(req.get(fmt::format("https://raw.githubusercontent.com/cdc-sys/level-thumbnails/main/thumbs/{}.png", level->m_levelID.value())));
    }
}

void RDDailyNode::downloadThumbnailFinished(CCImage* image) {
    if (this && m_menu) {
        auto size = m_mainNode->getContentSize();
        auto key = fmt::format("thumbnail-{}", m_currentLevel->m_levelID.value());
        CCTexture2D* texture = new CCTexture2D();
        texture->autorelease();
        texture->initWithImage(image);

        auto clippingNode = CCClippingNode::create();
        clippingNode->setAnchorPoint({ 0.5f, 0.5f });
        clippingNode->setPosition(m_innerBG->getPosition());
        clippingNode->setContentSize(m_innerBG->getScaledContentSize());
        clippingNode->setAlphaThreshold(0.03f);
        clippingNode->setID("thumbnail-node");

        auto stencil = CCScale9Sprite::create("square02b_001.png");
        stencil->setScale(0.5f);
        stencil->setPosition(clippingNode->getContentSize()/2);
        stencil->setContentSize({ size.width*2 - 30.f, size.height / 1.16f });

        auto sprite = CCSprite::createWithTexture(texture);
        sprite->setPosition(stencil->getPosition());
        sprite->setScale(stencil->getScaledContentWidth() / sprite->getContentWidth());
        sprite->setOpacity(0);

        clippingNode->setStencil(stencil);
        clippingNode->addChild(sprite);
        m_menu->addChild(clippingNode, 0);
        sprite->runAction(CCFadeIn::create(0.25f));

        auto overlay = CCScale9Sprite::create("innerBG_overlay.png"_spr);
        overlay->setPosition(m_innerBG->getPosition());
        overlay->setScale(m_innerBG->getScale());
        overlay->setContentSize(m_innerBG->getContentSize());
        m_menu->addChild(overlay, 1);    
    }
}

void RDDailyNode::downloadThumbnailFail() {
    auto failSprite = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    failSprite->setScale(0.35f);
    failSprite->setPosition(m_innerBG->getPosition() + m_innerBG->getScaledContentSize()/2 - failSprite->getScaledContentSize()/2 - ccp(2.f, 2.f));
    m_menu->addChild(failSprite);
}

void RDDailyNode::updateTimeLabel(float dt) {
    std::vector<int> timelyLeft = {Variables::DailyLeft, Variables::WeeklyLeft, Variables::EventLeft};
    if (timelyLeft[m_levelType] < 1) {
        m_timeLabel->setVisible(false);
        m_timeLeftLabel->setVisible(false);
        m_timerLoadingCircle->setVisible(true);
        this->unschedule(schedule_selector(RDDailyNode::updateTimeLabel));
    } else {
        m_timeLabel->setString(GameToolbox::getTimeString(timelyLeft[m_levelType], true).c_str());
        m_timeLabel->setVisible(true);
        m_timeLeftLabel->setVisible(true);
        m_timerLoadingCircle->setVisible(false);
    }

}

RDDailyNode* RDDailyNode::create(int levelType, CCSize size, std::string id, float scale) {
    auto ret = new RDDailyNode();
    ret->m_levelType = levelType;
    if (ret && ret->init(levelType, size, id, scale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}