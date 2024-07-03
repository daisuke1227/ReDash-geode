#include "RDDailyButton.hpp"

bool RDDailyButton::init(bool isWeekly, CCPoint position, CCSize size) {
    if (!CCNode::init()) return false;

    auto GLM = GameLevelManager::get();
    GLM->getGJDailyLevelState(GJTimedLevelType::Daily);
    GLM->getGJDailyLevelState(GJTimedLevelType::Weekly);

    auto background = CCScale9Sprite::create(isWeekly ? "GJ_square05.png" : "GJ_square01.png");
    background->setContentSize(size);
    background->setPosition({ size.width/2, size.height/2 });
    this->addChild(background);

    auto crownSprite = CCSprite::createWithSpriteFrameName(isWeekly ? "gj_weeklyCrown_001.png" : "gj_dailyCrown_001.png");
    crownSprite->setScale(0.75f);
    crownSprite->setPosition({ size.width/2, size.height + 8.f });
    this->addChild(crownSprite);

    auto titleSprite = CCSprite::createWithSpriteFrameName(isWeekly ? "weeklyLevelLabel_001.png" : "dailyLevelLabel_001.png");
    titleSprite->setScale(0.7f);
    titleSprite->setPosition({ size.width/2, size.height - 22.5f });
    this->addChild(titleSprite);

    auto innerBG = CCScale9Sprite::create("square02b_001.png");
    innerBG->setScale(0.5f);
    innerBG->setContentSize({ size.width - 15.f, size.height / 2.32f });
    innerBG->setContentSize(innerBG->getContentSize() * 2);
    innerBG->setPosition({ size.width/2, size.height/2 });
    innerBG->setColor({ 0, 0, 0 });
    innerBG->setOpacity(50);
    this->addChild(innerBG);

    auto menu = CCMenu::create();
    menu->setPosition({ 0.f, 0.f });
    menu->setContentSize(size);
    this->addChild(menu);

    auto viewButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png"),
        this,
        menu_selector(RDDailyButton::onView)
    );
    viewButton->setScale(0.6f);
    viewButton->m_baseScale = 0.6f;
    viewButton->m_scaleMultiplier = 1.15f;
    viewButton->setPosition({ size.width*5.5f/6.5f, size.height/2.f }); // dont ask...
    menu->addChild(viewButton);

    // 00 : na
    // 01 : easy
    // 02 : normal
    // 03 : hard
    // 04 : harder
    // 05 : insane
    // 06 : hard demon = 0 + 4 + 2
    // 07 : easy demon = m_demonDifficulty + 4
    // 08 : medium demon = m_demonDifficulty + 4
    // 09 : insane demon = m_demonDifficulty + 4
    // 10 : extreme demon = m_demonDifficulty + 4
    // auto : auto

    auto level = GLM->getSavedDailyLevel(isWeekly ? GLM->m_weeklyIDUnk : GLM->m_dailyIDUnk);
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

    auto difficultySprite = GJDifficultySprite::create(difficultyRating, GJDifficultyName::Short);
    difficultySprite->updateFeatureState(as<GJFeatureState>(featureRating));
    difficultySprite->setScale(0.8f);
    difficultySprite->setPosition({ size.width/7.5f, viewButton->getPositionY() + 2.5f });
    menu->addChild(difficultySprite);

    auto menuSize = innerBG->getScaledContentSize();
    auto baseY = innerBG->getPositionY() - menuSize.height/2;

    auto nameLabel = CCLabelBMFont::create(level->m_levelName.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.5f);
    nameLabel->setAnchorPoint({ 0, 0.5f });
    nameLabel->setPositionX(difficultySprite->getPositionX() + difficultySprite->getScaledContentWidth()/2 + 5.f);
    nameLabel->setPositionY(baseY + menuSize.height*3/4);
    menu->addChild(nameLabel);

    auto creatorLabel = CCLabelBMFont::create((std::string("by ") + level->m_creatorName).c_str(), "goldFont.fnt");
    creatorLabel->setAnchorPoint({ 0, 0.5f });
    creatorLabel->setScale(0.5f);
    creatorLabel->setPositionX(nameLabel->getPositionX());
    creatorLabel->setPositionY(baseY + menuSize.height/2);
    menu->addChild(creatorLabel);

    auto songLabel = CCLabelBMFont::create(MusicDownloadManager::sharedState()->getSongInfoObject(level->m_songID)->m_songName.c_str(), "bigFont.fnt");
    songLabel->setAnchorPoint({ 0, 0.5f });
    songLabel->setColor({ 250, 110, 245 });
    songLabel->setScale(0.35f);
    songLabel->setPositionX(nameLabel->getPositionX());
    songLabel->setPositionY(baseY + menuSize.height/4);
    menu->addChild(songLabel);

    auto starsLabel = CCLabelBMFont::create(std::to_string(level->m_stars).c_str(), "bigFont.fnt");
    starsLabel->setScale(0.32f);
    starsLabel->setPositionX(difficultySprite->getPositionX() - 5.f);
    starsLabel->setPositionY(baseY + menuSize.height/6);
    menu->addChild(starsLabel);

    auto starSprite = CCSprite::createWithSpriteFrameName("star_small01_001.png");
    starSprite->setScale(0.8f);
    starSprite->setPositionX(starsLabel->getPositionX() + starsLabel->getScaledContentWidth()/2 + 5.f);
    starSprite->setPositionY(baseY + menuSize.height/6);
    menu->addChild(starSprite);

    auto bonusBG = CCScale9Sprite::create("GJ_square02.png");
    bonusBG->setScale(0.5f);
    bonusBG->setContentSize({ size.width/2.5f, size.height/5.5f });
    bonusBG->setContentSize(bonusBG->getContentSize() * 2);
    bonusBG->setPosition({ size.width/5.f + 7.5f , size.height/11.f + 10.f });
    bonusBG->setColor({ 0, 0, 0 });
    bonusBG->setOpacity(50);
    this->addChild(bonusBG);

    auto bonusMenu = CCMenu::create();
    bonusMenu->setContentSize(bonusBG->getScaledContentSize());
    bonusMenu->setPosition(bonusBG->getPosition());
    bonusMenu->setPositionX(bonusMenu->getPositionX() + (isWeekly ? 5.f : 2.f));
    bonusMenu->setLayout(
        RowLayout::create()
        ->setGap(0.f)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setAutoScale(false)
    );
    this->addChild(bonusMenu);

    auto bonusSprite = isWeekly ? 
        CCSprite::createWithSpriteFrameName("chest_03_02_001.png") :
        CCSprite::createWithSpriteFrameName("GJ_bigDiamond_001.png");
    bonusSprite->setScale(isWeekly ? 0.16f : 0.4f);
    bonusMenu->addChild(bonusSprite);

    auto bonusLabel = CCLabelBMFont::create((isWeekly ? " / Bonus" : "x4 Bonus"), "bigFont.fnt");
    bonusLabel->setScale(0.4f);
    bonusMenu->addChild(bonusLabel);


    bonusMenu->updateLayout();
    
    auto time = isWeekly ? GLM->m_weeklyTimeLeft : GLM->m_dailyTimeLeft;
    auto label = CCLabelBMFont::create(std::to_string(time).c_str(), "goldFont.fnt");
    label->setScale(0.75f);
    label->setPosition({ size.width*4/5, size.height/6 });
    this->addChild(label);
    

    this->setPosition(position);
    this->setContentSize(size);

    return true;
}

void RDDailyButton::onView(CCObject* sender) {
    auto sc = LevelInfoLayer::scene(m_currentLevel, m_isWeekly);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
}

RDDailyButton* RDDailyButton::create(bool isWeekly, CCPoint position, CCSize size) {
    auto ret = new RDDailyButton();
    ret->m_isWeekly = isWeekly;
    if (ret && ret->init(isWeekly, position, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}