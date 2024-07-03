#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDMainButton : public CCMenuItemSpriteExtra {
public:
    static RDMainButton* create(CCPoint position, CCSize size, cocos2d::SEL_MenuHandler callback);
    bool init(CCPoint position, CCSize size, cocos2d::SEL_MenuHandler callback);
};