#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Variables.hpp"
using namespace geode::prelude;

class RDDailyNode : public CCNode {
public:
    EventListener<web::WebTask> m_listener;

    GJGameLevel* m_currentLevel;
    int m_levelType;

    CCNode* m_mainNode;
    CCMenu* m_menu;
    CCMenu* m_bonusMenu;
    CCScale9Sprite* m_innerBG;
    CCLabelBMFont* m_timeLabel;
    CCLabelBMFont* m_timeLeftLabel;
    LoadingCircle* m_loadingCircle;
    LoadingCircle* m_timerLoadingCircle;
    CCMenuItemSpriteExtra* m_skipButton;
    CCMenuItemSpriteExtra* m_reloadButton;
    CCMenuItemSpriteExtra* m_safeButton;

    void onView(CCObject* sender);
    void onSkiplevel(CCObject* sender);
    void onCreatorLabel(CCObject* sender);
    void onReload(CCObject* sender);
    void onTheSafe(CCObject* sender);
    void onClaimReward(CCObject* sender);

    void downloadLevelFailed();
    void updateTimeLabel(float time);
    void setupLevelMenu(GJGameLevel* level);
    void setupBonusMenu(GJGameLevel* level);

    void downloadThumbnailFinished(CCImage* image);
    void downloadThumbnailFail();

    static RDDailyNode* create(int levelType, CCSize size, std::string id, float scale);
    bool init(int levelType, CCSize size, std::string id, float scale);
};