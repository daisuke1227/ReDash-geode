#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../../Variables.hpp"
using namespace geode::prelude;

class RDTimelyNode : public CCNode {
public:
    EventListener<web::WebTask> m_listener;

    GJGameLevel* m_currentLevel;

    CCNode* m_mainNode;
    CCMenu* m_menu;
    CCMenu* m_bonusMenu;
    CCScale9Sprite* m_innerBG;
    CCLabelBMFont* m_timeLabel;
    CCLabelBMFont* m_timeLeftLabel;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_viewButton;
    CCMenuItemSpriteExtra* m_skipButton;
    CCMenuItemSpriteExtra* m_reloadButton;
    CCMenuItemSpriteExtra* m_safeButton;
    
    void onView(CCObject* sender);
    void onCreatorLabel(CCObject* sender);
    
    void downloadLevelFailed();
    void downloadThumbnailFinished(CCImage* image);
    void downloadThumbnailFail();

    virtual bool init(CCSize size, std::string id, float scale);
    virtual CCScale9Sprite* createBG(CCSize size, float scale) = 0;
    virtual void onSkipLevel(CCObject* sender) = 0;
    virtual void onReload(CCObject* sender) = 0;
    virtual void onTheSafe(CCObject* sender) = 0;
    virtual void onClaimReward(CCObject* sender) = 0;

    virtual void setupLevelMenu(GJGameLevel* level);
    virtual void setupBonusMenu(GJGameLevel* level) = 0;


};