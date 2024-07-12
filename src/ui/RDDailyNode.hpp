#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Variables.hpp"
using namespace geode::prelude;

class RDDailyNode : public CCNode {
public:
    EventListener<web::WebTask> m_listener;
    ByteVector m_thumbnailData;

    GJGameLevel* m_currentLevel;
    bool m_isWeekly;

    CCMenu* m_menu;
    CCScale9Sprite* m_innerBG;
    CCLabelBMFont* m_timeLabel;
    CCLabelBMFont* m_timeLeftLabel;
    LoadingCircle* m_loadingCircle;
    LoadingCircle* m_timerLoadingCircle;
    CCMenuItemSpriteExtra* m_skipButton;
    CCMenuItemSpriteExtra* m_reloadButton;

    void onView(CCObject* sender);
    void onSkiplevel(CCObject* sender);
    void onCreatorLabel(CCObject* sender);
    void onReload(CCObject* sender);
    void onTheSafe(CCObject* sender);
    void onClaimReward(CCObject* sender);

    void downloadLevelFailed();
    void updateTimeLabel(float time);
    void setupLevelMenu(GJGameLevel* level);

    void downloadThumbnailFinished();
    void downloadThumbnailFail();

    static RDDailyNode* create(bool isWeekly, CCPoint position, CCSize size, std::string id);
    bool init(bool isWeekly, CCPoint position, CCSize size, std::string id);
};