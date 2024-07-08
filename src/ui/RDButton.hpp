#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDButton : public CCMenuItemSpriteExtra {
public:
    LoadingCircle* m_loadingCircle;
    CCLabelBMFont* m_descLabel;

    void updateLeaderboardLabel();

    static RDButton* create(CCObject* target, std::string title, std::string description, std::string sprite, cocos2d::SEL_MenuHandler callback, std::string id);
    bool init(CCObject* target, std::string title, std::string description, std::string sprite, cocos2d::SEL_MenuHandler callback, std::string id);
};