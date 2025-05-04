// RDDailyNode.cpp
#include <Geode/Geode.hpp>
#include <Geode/Fields.hpp>
#include "RDDailyNode.hpp"
#include "../hooks/GameLevelManager.hpp"

using namespace geode::prelude;

RDDailyNode* RDDailyNode::create(CCSize size, std::string id, float scale) {
    auto ret = new RDDailyNode();
    if (ret && ret->init(size, id, scale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void RDDailyNode::updateTimeLabel(float) {
    if (Variables::DailyLeft < 1) {
        m_timeLabel->setVisible(false);
        m_timeLeftLabel->setVisible(false);
        m_timerLoadingCircle->setVisible(true);
        this->unschedule(schedule_selector(RDDailyNode::updateTimeLabel));
    } else {
        m_timeLabel->setString(GameToolbox::getTimeString(Variables::DailyLeft, true).c_str());
        // adjust scale if needed...
        m_timeLabel->setVisible(true);
        m_timeLeftLabel->setVisible(true);
        m_timerLoadingCircle->setVisible(false);
    }
}

bool RDDailyNode::init(CCSize size, std::string id, float scale) {
    if (!RDTimelyNode::init(size, id, scale)) return false;
    auto GLM = GameLevelManager::get();
    auto &fields = geode::getFields<MyGLM>(GLM);

    // Crown and title sprites...
    auto crown = CCSprite::createWithSpriteFrameName("gj_dailyCrown_001.png");
    crown->setScale(0.75f);
    crown->setPosition({ size.width/2, size.height + 8.f });
    m_mainNode->addChild(crown);

    auto title = CCSprite::createWithSpriteFrameName("dailyLevelLabel_001.png");
    title->setScale(0.7f);
    title->setPosition({ size.width/2, size.height - 22.5f });
    m_mainNode->addChild(title);

    // Bonus menu offset
    m_bonusMenu->setPositionX(m_bonusMenu->getPositionX() + 2.f);

    // Load or download level
    if (auto level = GLM->getSavedDailyLevel(fields.m_dailyIDUnk)) {
        setupLevelMenu(level);
    } else {
        GLM->downloadLevel(-1, false);
        m_loadingCircle->setVisible(true);
        m_bonusMenu->setVisible(false);
    }

    // Time labels
    int t = std::max(0, Variables::DailyLeft);
    m_timeLabel = CCLabelBMFont::create(
        GameToolbox::getTimeString(t, true).c_str(),
        Mod::get()->getSettingValue<bool>("use-pusab") ? "bigFont.fnt" : "gjFont16.fnt"
    );
    m_timeLabel->setScale(0.55f);
    m_timeLabel->setAnchorPoint({1,0.5f});
    m_timeLabel->setPosition({ m_innerBG->getPositionX() + m_innerBG->getScaledContentSize().width/2, size.height/9 });
    m_mainNode->addChild(m_timeLabel);

    m_timeLeftLabel = CCLabelBMFont::create("New Daily in:", "bigFont.fnt");
    m_timeLeftLabel->setScale(0.3f);
    m_timeLeftLabel->setAnchorPoint({1,0.5f});
    m_timeLeftLabel->setPosition({ m_timeLabel->getPositionX(),
        (m_safeButton->getPositionY() + m_innerBG->getPositionY() - m_innerBG->getScaledContentHeight()/2)/2 });
    m_timeLeftLabel->setColor({200,200,200});
    m_mainNode->addChild(m_timeLeftLabel);

    m_timerLoadingCircle = LoadingCircle::create();
    m_timerLoadingCircle->setScale(0.45f);
    m_timerLoadingCircle->setVisible(false);
    m_mainNode->addChild(m_timerLoadingCircle);

    if (t == 0) {
        m_timeLabel->setVisible(false);
        m_timeLeftLabel->setVisible(false);
        m_timerLoadingCircle->setVisible(true);
    }

    return true;
}

void RDDailyNode::setupLevelMenu(GJGameLevel* level) {
    RDTimelyNode::setupLevelMenu(level);
    auto GLM = GameLevelManager::get();
    auto &fields = geode::getFields<MyGLM>(GLM);

    auto skipBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"),
        this, menu_selector(RDDailyNode::onSkipLevel)
    );
    skipBtn->setVisible(fields.m_dailyIDUnk < GLM->m_dailyID);
    skipBtn->setScale(0.5f);
    skipBtn->setPositionX(m_innerBG->getPositionX() - m_innerBG->getScaledContentSize().width/2 + 5.f);
    skipBtn->setPositionY(m_innerBG->getPositionY() + m_innerBG->getScaledContentSize().height/2);
    m_menu->addChild(skipBtn);
    m_skipButton = skipBtn;

    if (level->m_normalPercent.value() == 100
        && !GameStatsManager::sharedState()->hasCompletedDailyLevel(fields.m_dailyIDUnk)) {
        auto claimBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_rewardBtn_001.png"),
            this, menu_selector(RDDailyNode::onClaimReward)
        );
        claimBtn->setScale(0.6f);
        claimBtn->setPosition(m_viewButton->getPosition() + ccp(10,0));
        m_menu->addChild(claimBtn);
        skipBtn->setVisible(false);
    }
}

void RDDailyNode::onSkipLevel(CCObject*) {
    auto GLM = GameLevelManager::get();
    auto &fields = geode::getFields<MyGLM>(GLM);

    geode::createQuickPopup(
        "Skip level",
        "There is a <cy>new</c> daily level available.\nSkip?",
        "Cancel","Skip",
        [this,&fields](auto, bool ok){
            if (ok) {
                m_loadingCircle->setVisible(true);
                m_menu->removeAllChildren();
                m_bonusMenu->setVisible(false);
                GameLevelManager::get()->downloadLevel(-1,false);
                Mod::get()->setSavedValue("claimed-daily",false);
                fields.m_dailyIDUnk = 0;
            }
        }
    );
}

void RDDailyNode::onReload(CCObject*) {
    m_loadingCircle->setVisible(true);
    m_menu->removeAllChildren();
    if (Variables::DailyLeft == 0)
        GameLevelManager::get()->getGJDailyLevelState(GJTimedLevelType::Daily);
    GameLevelManager::get()->downloadLevel(-1,false);
}

void RDDailyNode::onTheSafe(CCObject*) {
    auto search = GJSearchObject::create(SearchType::DailySafe);
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(0.5f, LevelBrowserLayer::scene(search))
    );
}

void RDDailyNode::onClaimReward(CCObject* sender) {
    auto GLM = GameLevelManager::get();
    auto &fields = geode::getFields<MyGLM>(GLM);

    Mod::get()->setSavedValue("claimed-daily", true);
    // show reward UI...
    m_skipButton->setVisible(fields.m_dailyIDUnk < GLM->m_dailyID);
}