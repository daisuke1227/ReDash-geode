#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDButton : public CCMenuItemSpriteExtra {
public:
    LoadingCircle* m_loadingCircle;
    CCMenu* m_labelMenu;
    CCSprite* m_titleSprite;

    void updateLeaderboardLabel();
    void getLeaderboardRankFailed();

    static RDButton* create(CCObject* target, std::string title, std::initializer_list<std::string> description, std::string sprite, float spriteScale, cocos2d::SEL_MenuHandler callback, std::string id);
    bool init(CCObject* target, std::string title, std::initializer_list<std::string> description, std::string sprite, float spriteScale, cocos2d::SEL_MenuHandler callback, std::string id);
};