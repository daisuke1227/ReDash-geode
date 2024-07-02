#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDButton : public CCMenuItemSpriteExtra {
public:
    static RDButton* create(CCObject* target, std::string title, std::string description, std::string sprite, cocos2d::SEL_MenuHandler callback);
    bool init(CCObject* target, std::string title, std::string description, std::string sprite, cocos2d::SEL_MenuHandler callback);
};