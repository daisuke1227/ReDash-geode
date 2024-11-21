#pragma once

#include <Geode/Geode.hpp>
#include "RDTimelyNode.hpp"
#include "../../Variables.hpp"
using namespace geode::prelude;

class RDDailyNode : public RDTimelyNode {
public:
    LoadingCircle* m_timerLoadingCircle;

    static RDDailyNode* create(CCSize size, std::string id, float scale);

    void updateTimeLabel(float time);
    int getBonusDiamonds();

    virtual bool init(CCSize size, std::string id, float scale) override;
    virtual CCScale9Sprite* createBG(CCSize size, float scale) override;
    virtual void onSkipLevel(CCObject* sender) override;
    virtual void onReload(CCObject* sender) override;
    virtual void onTheSafe(CCObject* sender) override;
    virtual void onClaimReward(CCObject* sender) override;

    virtual void setupLevelMenu(GJGameLevel* level) override;
    virtual void setupBonusMenu(GJGameLevel* level) override;
};