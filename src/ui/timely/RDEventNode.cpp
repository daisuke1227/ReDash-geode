// RDEventNode.cpp
#include <Geode/Geode.hpp>
#include <Geode/modify/Fields.hpp>
#include "RDEventNode.hpp"
#include "../hooks/GameLevelManager.hpp"

using namespace geode::prelude;

RDEventNode* RDEventNode::create(CCSize size, std::string id, float scale) {
    auto ret = new RDEventNode();
    if (ret && ret->init(size, id, scale)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool RDEventNode::init(CCSize size, std::string id, float scale) {
    if (!RDTimelyNode::init(size, id, scale)) return false;
    auto GLM = GameLevelManager::get();
    auto& fields = geode::getFields<MyGLM>(GLM);

    auto crown = CCSprite::createWithSpriteFrameName("RD_eventCrown_001.png"_spr);
    crown->setScale(0.75f);
    crown->setPosition({ size.width/2, size.height + 8.f });
    m_mainNode->addChild(crown);

    auto title = CCSprite::createWithSpriteFrameName("eventLevelLabel_001.png");
    title->setScale(0.7f);
    title->setPosition({ size.width/2, size.height - 22.5f });
    m_mainNode->addChild(title);

    m_bonusMenu->setPositionX(m_bonusMenu->getPositionX() + 5.f);

    if (auto level = GLM->getSavedDailyLevel(fields.m_eventIDUnk)) {
        setupLevelMenu(level);
    } else {
        GLM->downloadLevel(-3,false);
        m_loadingCircle->setVisible(true);
        m_bonusMenu->setVisible(false);
    }

    return true;
}

void RDEventNode::setupLevelMenu(GJGameLevel* level) {
    RDTimelyNode::setupLevelMenu(level);
    auto GLM = GameLevelManager::get();
    auto& fields = geode::getFields<MyGLM>(GLM);

    auto skipBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"),
        this, menu_selector(RDEventNode::onSkipLevel)
    );
    skipBtn->setVisible(fields.m_eventIDUnk < GLM->m_eventID);
    skipBtn->setScale(0.5f);
    skipBtn->setPositionX(m_innerBG->getPositionX() - m_innerBG->getScaledContentSize().width/2 + 5.f);
    skipBtn->setPositionY(m_innerBG->getPositionY() + m_innerBG->getScaledContentSize().height/2);
    m_menu->addChild(skipBtn);
    m_skipButton = skipBtn;

    if (level->m_normalPercent.value() == 100
        && !GameStatsManager::sharedState()->hasCompletedDailyLevel(fields.m_eventIDUnk)) {
        auto claimBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_rewardBtn_001.png"),
            this, menu_selector(RDEventNode::onClaimReward)
        );
        claimBtn->setScale(0.6f);
        claimBtn->setPosition(m_viewButton->getPosition() + ccp(10,0));
        m_menu->addChild(claimBtn);
        skipBtn->setVisible(false);
    }
}

void RDEventNode::onSkipLevel(CCObject*) {
    auto GLM = GameLevelManager::get();
    auto& fields = geode::getFields<MyGLM>(GLM);

    geode::createQuickPopup(
        "Skip event",
        "New event level available. Skip?",
        "Cancel","Skip",
        [this,&fields](auto, bool ok){
            if (ok) {
                m_loadingCircle->setVisible(true);
                m_menu->removeAllChildren();
                m_bonusMenu->setVisible(false);
                GLM->downloadLevel(-3,false);
                Mod::get()->setSavedValue("claimed-event",false);
                fields.m_eventIDUnk = 0;
            }
        }
    );
}

// onReload, onTheSafe, onClaimReward analogous to daily but using event IDs