#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class MiniButton : public CCMenuItemSpriteExtra {
public:
    static MiniButton* create(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id);
    bool init(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id);
};

class MiniRDButton : public CCMenuItemSpriteExtra {
public:
    void rotateIcon(float rotation);

    static MiniRDButton* create(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id);
    bool init(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id);
};