#include "RDDailyNode.hpp"

bool RDDailyNode::init(bool isWeekly, CCPoint position, CCSize size, std::string id) {
    if (!CCNode::init()) return false;

    auto GLM = GameLevelManager::get();

    auto background = CCScale9Sprite::create(isWeekly ? "GJ_square05.png" : "GJ_square01.png");
    background->setContentSize(size);
    background->setPosition({ size.width/2, size.height/2 });
    background->setID("background");
    this->addChild(background, 0);

    auto crownSprite = CCSprite::createWithSpriteFrameName(isWeekly ? "gj_weeklyCrown_001.png" : "gj_dailyCrown_001.png");
    crownSprite->setScale(0.75f);
    crownSprite->setPosition({ size.width/2, size.height + 8.f });
    crownSprite->setID("crown-sprite");
    this->addChild(crownSprite, 1);

    auto titleSprite = CCSprite::createWithSpriteFrameName(isWeekly ? "weeklyLevelLabel_001.png" : "dailyLevelLabel_001.png");
    titleSprite->setScale(0.7f);
    titleSprite->setPosition({ size.width/2, size.height - 22.5f });
    titleSprite->setID("title-sprite");
    this->addChild(titleSprite, 1);

    auto innerBG = CCScale9Sprite::create("square02b_001.png");
    innerBG->setScale(0.5f);
    innerBG->setContentSize({ size.width*2 - 30.f, size.height / 1.16f });
    innerBG->setPosition({ size.width/2, size.height/2 });
    innerBG->setColor({ 0, 0, 0 });
    innerBG->setOpacity(50);
    innerBG->setID("inner-background");
    this->addChild(innerBG, 1);
    m_innerBG = innerBG;

    auto menu = CCMenu::create();
    menu->setPosition({ 0.f, 0.f });
    menu->setContentSize(size);
    menu->setID("level-menu");
    this->addChild(menu, 2);
    m_menu = menu;

    auto loadingCircle = LoadingCircle::create();
    loadingCircle->setScale(0.75f);
    loadingCircle->setContentSize({ 0 , 0 });
    loadingCircle->setPosition({ size.width/2, size.height/2 });
    loadingCircle->setVisible(false);
    loadingCircle->m_sprite->setPosition({ 0 , 0 });
    loadingCircle->m_sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1, 360)));
    loadingCircle->setID("loading-circle");
    this->addChild(loadingCircle, 2);
    m_loadingCircle = loadingCircle;

    if (auto level = GLM->getSavedDailyLevel(m_isWeekly ? GLM->m_weeklyIDUnk : GLM->m_dailyIDUnk)) {
        RDDailyNode::setupLevelMenu(level);
    } else {
        GLM->downloadLevel(m_isWeekly ? -2 : -1, false);
        m_loadingCircle->setVisible(true);
    }

    auto safeMenu = CCMenu::create();
    safeMenu->setPosition({ 0.f, 0.f });
    safeMenu->setContentSize(size);
    safeMenu->setID("safe-button-menu");
    this->addChild(safeMenu, 2);

    auto safeButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_safeBtn_001.png"),
        this,
        menu_selector(RDDailyNode::onTheSafe)
    );
    safeButton->setScale(0.4f);
    safeButton->setPosition(size);
    safeButton->m_scaleMultiplier = 1.15f;
    safeButton->m_baseScale = 0.5f;
    safeButton->setID("safe-button");
    this->addChild(safeButton);
    safeMenu->addChild(safeButton);

    auto bonusBG = CCScale9Sprite::create("GJ_square02.png");
    bonusBG->setScale(0.5f);
    bonusBG->setContentSize({ size.width/2.5f, size.height/5.5f });
    bonusBG->setContentSize(bonusBG->getContentSize() * 2);
    bonusBG->setPosition({ size.width/5.f + 7.5f , size.height/11.f + 10.f });
    bonusBG->setColor({ 0, 0, 0 });
    bonusBG->setOpacity(50);
    bonusBG->setID("bonus-background");
    this->addChild(bonusBG, 1);

    auto bonusMenu = CCMenu::create();
    bonusMenu->setContentSize(bonusBG->getScaledContentSize());
    bonusMenu->setPosition(bonusBG->getPosition());
    bonusMenu->setPositionX(bonusMenu->getPositionX() + (isWeekly ? 5.f : 2.f));
    bonusMenu->setID("bonus-menu");
    bonusMenu->setLayout(
        RowLayout::create()
        ->setGap(0.f)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setAutoScale(false)
    );
    this->addChild(bonusMenu, 2);

    auto bonusSprite = isWeekly ? 
        CCSprite::createWithSpriteFrameName("chest_03_02_001.png") :
        CCSprite::createWithSpriteFrameName("GJ_bigDiamond_001.png");
    bonusSprite->setScale(isWeekly ? 0.16f : 0.4f);
    bonusSprite->setID("bonus-sprite");
    bonusMenu->addChild(bonusSprite);

    auto bonusLabel = CCLabelBMFont::create((isWeekly ? " / Bonus" : "x4 Bonus"), "bigFont.fnt");
    bonusLabel->setScale(0.4f);
    bonusLabel->setID("bonus-label");
    bonusMenu->addChild(bonusLabel);

    bonusMenu->updateLayout();

    
    auto time = (m_isWeekly ? Variables::WeeklyLeft : Variables::DailyLeft);
    if (time < 0) time = 0;
    auto timeLabel = CCLabelBMFont::create(GameToolbox::getTimeString(time, true).c_str(), Mod::get()->getSettingValue<bool>("use-pusab") ? "bigFont.fnt" : "gjFont16.fnt");
    timeLabel->setScale(0.55f);
    timeLabel->setAnchorPoint({ 1, 0.5f });
    timeLabel->setPosition({ innerBG->getPositionX() + innerBG->getScaledContentSize().width/2 , size.height/9 });
    timeLabel->setID("time-label");
    this->addChild(timeLabel, 1);
    m_timeLabel = timeLabel;

    auto timeLeftLabel = CCLabelBMFont::create(isWeekly ? "New Weekly in:" : "New Daily in:", "bigFont.fnt");
    timeLeftLabel->setScale(0.3f);
    timeLeftLabel->setAnchorPoint({ 1, 0.5f });
    timeLeftLabel->setPosition({ timeLabel->getPositionX(), timeLabel->getPositionY() + timeLabel->getScaledContentHeight()/2 + timeLeftLabel->getScaledContentHeight()/2});
    timeLeftLabel->setColor({ 200, 200, 200 });
    timeLeftLabel->setID("time-left-label");
    this->addChild(timeLeftLabel, 1);
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
    this->addChild(timerLoadingCircle, 2);
    m_timerLoadingCircle = timerLoadingCircle;

    if ((m_isWeekly ? Variables::WeeklyLeft : Variables::DailyLeft) == 0) {
        timeLabel->setVisible(false);
        timeLeftLabel->setVisible(false);
        timerLoadingCircle->setVisible(true);
    }

    if ((isWeekly ? Variables::WeeklyLeft : Variables::DailyLeft) > 0) {
        this->schedule(schedule_selector(RDDailyNode::updateTimeLabel), 1.f);
    }

    this->setPosition(position);
    this->setContentSize(size);
    this->setID(id);

    return true;
}

void RDDailyNode::onView(CCObject* sender) {
    auto sc = LevelInfoLayer::scene(m_currentLevel, false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
}

void RDDailyNode::onSkiplevel(CCObject* sender) {
    geode::createQuickPopup(
        "Skip level",
        fmt::format("There is a <cy>new</c> {} level available.\nSkip the current level and load the next?", m_isWeekly ? "weekly" : "daily"),
        "Cancel", "Skip",
        [this](auto, bool btn2) {
            if (btn2) {
                m_loadingCircle->setVisible(true);
                m_menu->removeAllChildren();
                GameLevelManager::get()->downloadLevel(m_isWeekly ? -2 : -1, false);
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

    if (m_isWeekly ? Variables::WeeklyLeft : Variables::DailyLeft == 0) GameLevelManager::get()->getGJDailyLevelState(m_isWeekly ? GJTimedLevelType::Weekly : GJTimedLevelType::Daily);
    GameLevelManager::get()->downloadLevel(m_isWeekly ? -2 : -1, false);
}

void RDDailyNode::onTheSafe(CCObject* sender) {
    auto search = GJSearchObject::create(m_isWeekly ? SearchType::WeeklySafe : SearchType::DailySafe);
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

void RDDailyNode::setupLevelMenu(GJGameLevel* level) {
    auto GLM = GameLevelManager::get();

    m_menu->removeAllChildren();
    m_loadingCircle->setVisible(false);

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

    if (m_isWeekly ? GLM->m_weeklyIDUnk < GLM->m_weeklyID : GLM->m_dailyIDUnk < GLM->m_dailyID) {
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
    m_menu->addChild(nameLabel, 1);

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
    m_menu->addChild(creatorButton, 1);

    auto songLabel = CCLabelBMFont::create(MusicDownloadManager::sharedState()->getSongInfoObject(level->m_songID)->m_songName.c_str(), "bigFont.fnt");
    songLabel->setScale(0.35f);
    if (songLabel->getScaledContentWidth() > maxX) {
        songLabel->setScale(maxX / songLabel->getContentWidth());
    }
    songLabel->setAnchorPoint({ 0, 0.5f });
    songLabel->setColor({ 250, 110, 245 });
    songLabel->setPositionX(nameLabel->getPositionX());
    songLabel->setPositionY(baseY + menuSize.height/4);
    songLabel->setID("song-label");
    m_menu->addChild(songLabel, 1);

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
    if (auto fakeTexture = CCTextureCache::get()->textureForKey(fmt::format("thumbnail-{}", level->m_levelID.value()).c_str())) {
        onDownloadThumbnailFinished();
    } else {
        m_listener.bind([this] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                if (!res->ok()) {
                    onDownloadThumbnailFail();
                } else {
                    auto data = res->data();
                    m_thumbnailData = data;
                    onDownloadThumbnailFinished();
                }
            }
        });

        auto req = web::WebRequest();
        m_listener.setFilter(req.get(fmt::format("https://raw.githubusercontent.com/cdc-sys/level-thumbnails/main/thumbs/{}.png", level->m_levelID.value())));
    }
}

void RDDailyNode::onDownloadThumbnailFinished() {
    auto thread = std::thread([this]() {
        auto image = Ref(new CCImage());
        image->initWithImageData(const_cast<uint8_t*>(m_thumbnailData.data()), m_thumbnailData.size());
        geode::Loader::get()->queueInMainThread([this, image](){
            auto size = this->getContentSize();
            auto key = fmt::format("thumbnail-{}", m_currentLevel->m_levelID.value());
            auto texture = CCTextureCache::get()->addUIImage(image, key.c_str());

            auto clippingNode = CCClippingNode::create();
            clippingNode->setAnchorPoint({ 0.5f, 0.5f });
            clippingNode->setPosition(m_innerBG->getPosition());
            clippingNode->setContentSize(m_innerBG->getScaledContentSize());
            clippingNode->setAlphaThreshold(0.5);
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

            image->release();
        });
    });
    thread.detach();
}

void RDDailyNode::onDownloadThumbnailFail() {
    auto failSprite = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    failSprite->setScale(0.35f);
    failSprite->setPosition(m_innerBG->getPosition() + m_innerBG->getScaledContentSize()/2 - failSprite->getScaledContentSize()/2 - ccp(2.f, 2.f));
    m_menu->addChild(failSprite);
}

void RDDailyNode::updateTimeLabel(float dt) {
    m_timeLabel->setString(GameToolbox::getTimeString((m_isWeekly ? Variables::WeeklyLeft : Variables::DailyLeft), true).c_str());
    m_timeLabel->setVisible(true);
    m_timeLeftLabel->setVisible(true);
    m_timerLoadingCircle->setVisible(false);
}

RDDailyNode* RDDailyNode::create(bool isWeekly, CCPoint position, CCSize size, std::string id) {
    auto ret = new RDDailyNode();
    ret->m_isWeekly = isWeekly;
    if (ret && ret->init(isWeekly, position, size, id)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}