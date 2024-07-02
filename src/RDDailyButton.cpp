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
    innerBG->setContentSize({ size.width - 15.f, size.height / 2.32f });
    innerBG->setPosition({ size.width/2, size.height/2 });
    innerBG->setColor({ 0, 0, 0 });
    innerBG->setOpacity(50);
    this->addChild(innerBG);

    auto menu = CCMenu::create();
    menu->setPosition({ 0.f, 0.f });
    menu->setContentSize(size);
    this->addChild(menu);

    auto button = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png"),
        this,
        menu_selector(RDDailyButton::onView)
    );
    button->setScale(0.6f);
    button->setPosition({ size.width*5.5f/6.5f, size.height/2.f }); // dont ask...
    menu->addChild(button);

    
    auto string = isWeekly ? (std::to_string(GLM->m_weeklyID) + "\n" + std::to_string(GLM->m_weeklyIDUnk)) : (std::to_string(GLM->m_dailyID) + "\n" + std::to_string(GLM->m_dailyIDUnk));
    auto label = CCLabelBMFont::create(string.c_str(), "goldFont.fnt");
    label->setPosition({ size.width/2, size.height/2 });
    this->addChild(label);

    auto bonusBG = CCScale9Sprite::create("GJ_square02.png");
    bonusBG->setContentSize({ size.width/2.5f, size.height/5.5f });
    bonusBG->setPosition({ size.width/5.f + 7.5f , size.height/11.f + 10.f });
    bonusBG->setColor({ 0, 0, 0 });
    bonusBG->setOpacity(50);
    this->addChild(bonusBG);

    this->setPosition(position);
    this->setContentSize(size);

    return true;
}

void RDDailyButton::onView(CCObject* sender) {
    log::info("RDDailyButton::onView");
}

RDDailyButton* RDDailyButton::create(bool isWeekly, CCPoint position, CCSize size) {
    auto ret = new RDDailyButton();
    if (ret && ret->init(isWeekly, position, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}