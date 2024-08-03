#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDButton : public CCMenuItemSpriteExtra {
public:
    LoadingCircle* m_loadingCircle;
    CCMenu* m_labelMenu;
    CCSprite* m_titleSprite;

    int m_completedQuests;

    void updateLeaderboardLabel();
    void getLeaderboardRankFailed();
    void rotateIcon(float rotation);
    void updateQuestsLabel();

    static RDButton* create(CCObject* target, std::string title, std::vector<std::string> description, std::string sprite, float spriteScale, cocos2d::SEL_MenuHandler callback, std::string id);
    bool init(CCObject* target, std::string title, std::vector<std::string> description, std::string sprite, float spriteScale, cocos2d::SEL_MenuHandler callback, std::string id);
};