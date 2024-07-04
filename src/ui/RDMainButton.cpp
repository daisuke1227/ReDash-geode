#include "RDMainButton.hpp"

bool RDMainButton::init(CCPoint position, CCSize size, cocos2d::SEL_MenuHandler callback) {
    auto GSM = GameStatsManager::sharedState();
    auto GLM = GameLevelManager::get();
    auto spriteNode = CCNode::create();

    if (!CCMenuItemSpriteExtra::init(spriteNode, nullptr, this, callback)) return false;

    auto background = CCScale9Sprite::create("GJ_square02.png");
    background->setContentSize(size);
    background->setPosition({ size.width/2, size.height/2 });
    spriteNode->addChild(background);

    auto crownSprite = CCSprite::createWithSpriteFrameName("RD_mainLevel.png"_spr);
    crownSprite->setScale(0.3f);
    crownSprite->setPosition({ size.width/2, size.height + 3.f });
    spriteNode->addChild(crownSprite);

    auto titleSprite = CCSprite::createWithSpriteFrameName("RD_mainLevelsLabel.png"_spr);
    titleSprite->setScale(0.58f);
    titleSprite->setPosition({ size.width/2, size.height - 22.5f });
    spriteNode->addChild(titleSprite);


    auto innerBG = CCScale9Sprite::create("square02b_001.png");
    innerBG->setScale(0.5f);
    innerBG->setContentSize({ size.width - 15.f, size.height / 2.32f });
    innerBG->setContentSize(innerBG->getContentSize() * 2);
    innerBG->setPosition({ size.width/2, size.height/2 });
    innerBG->setColor({ 0, 0, 0 });
    innerBG->setOpacity(125);
    spriteNode->addChild(innerBG);

    auto level = GLM->getMainLevel(Mod::get()->getSavedValue<int64_t>("last-main-level", 22), true);

    auto xd = CCLabelBMFont::create(fmt::format("{}\n{}\n{}\n{}", level->m_levelName, level->m_stars.value(), level->m_normalPercent.value(), level->m_orbCompletion.value()).c_str(), "bigFont.fnt");
    xd->setScale(0.5f);
    xd->setPosition({ size.width/2, size.height/2 });
    spriteNode->addChild(xd);


    auto lastPlayedLabel = CCLabelBMFont::create("Last Played:", "goldFont.fnt");
    lastPlayedLabel->setScale(0.4f);
    lastPlayedLabel->setPosition({ size.width/2, innerBG->getPositionY() + innerBG->getScaledContentHeight()/2 + 2.f });
    spriteNode->addChild(lastPlayedLabel);

    std::vector<int> mainLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 5001, 5002, 5003, 5004 };
    int mainLevelsCompleted = 0;
    for (auto level : mainLevels) {
        if (GSM->hasCompletedMainLevel(level)) mainLevelsCompleted++;
    }
    
    auto completedLabel = CCLabelBMFont::create("Completed Levels:", "goldFont.fnt");
    completedLabel->setScale(0.5f);
    completedLabel->setPosition({ size.width/2, size.height/4 });
    spriteNode->addChild(completedLabel);

    auto progressBG = CCSprite::create("RD_progressBar.png"_spr);
    progressBG->setScale(1.2f);
    progressBG->setPosition({ size.width/2, size.height/8.f });
    progressBG->setColor({ 0, 0, 0 });
    progressBG->setOpacity(125);
    spriteNode->addChild(progressBG);

    auto progressBar = CCSprite::create("RD_progressBar.png"_spr);
    progressBar->setScaleX(0.965f);
    progressBar->setScaleY(0.8f);
    progressBar->setPosition({ progressBG->getContentWidth()/2, progressBG->getContentHeight()/2 });

    auto clippingNode = CCClippingNode::create();
    auto stencil = CCScale9Sprite::create("square02_001.png");
	stencil->setAnchorPoint({ 0, 0.5f });
	stencil->setContentWidth(progressBar->getContentWidth());
    stencil->setContentHeight(100);
	stencil->setScaleX(mainLevelsCompleted / (float)mainLevels.size());
    clippingNode->setStencil(stencil);
    clippingNode->setAnchorPoint({ 0, 0 });
	clippingNode->setPosition({ 0, 0 });
	clippingNode->setContentSize(progressBG->getContentSize());

	clippingNode->addChild(progressBar);
    progressBG->addChild(clippingNode);

    auto progressLabel = CCLabelBMFont::create(fmt::format("{}/{}", mainLevelsCompleted, mainLevels.size()).c_str(), "bigFont.fnt");
    progressLabel->setScale(0.5f);
    progressLabel->setPosition({ size.width/2, size.height/8 + 1});
    spriteNode->addChild(progressLabel);

    this->setContentSize(size);
    this->setPosition(position + size/2);
	this->m_scaleMultiplier = 1.1f;

    return true;
}

RDMainButton* RDMainButton::create(CCPoint position, CCSize size, cocos2d::SEL_MenuHandler callback) {
    auto ret = new RDMainButton();
    if (ret && ret->init(position, size, callback)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}