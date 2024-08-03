#include "MiniButtons.hpp"

bool MiniButton::init(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id) {
    auto bgSprite = CCScale9Sprite::create("RD_square.png"_spr);
    bgSprite->setScale(0.3f);
    bgSprite->setColor(color);
    bgSprite->setID("bg-sprite");

    auto sprite = CCSprite::createWithSpriteFrameName(iconSprite.c_str());
    sprite->setPosition(bgSprite->getContentSize() / 2);
    sprite->setScale(iconScale);
    sprite->setID("icon-sprite");
    bgSprite->addChild(sprite);

    if (!CCMenuItemSpriteExtra::init(bgSprite, nullptr, target, callback)) return false;

    this->setID(id);

    return true;
}

MiniButton* MiniButton::create(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id) {
    auto ret = new MiniButton;

    if (ret && ret->init(iconSprite, iconScale, color, target, callback, id)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MiniRDButton::init(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id) {
    auto bgSprite = CCScale9Sprite::create("RD_square.png"_spr);
    bgSprite->setContentWidth(bgSprite->getContentWidth() / 80 * 150);
    bgSprite->setScale(0.3f);
    bgSprite->setColor(color);
    bgSprite->setID("bg-sprite");

    auto sprite = CCSprite::createWithSpriteFrameName(iconSprite.c_str());
    sprite->setPosition(bgSprite->getContentSize()/2 + ccp(bgSprite->getContentWidth()/2 - sprite->getContentWidth()/2 - 0.5f, 0));
    if (id == "mappacks-button") sprite->setPositionX(sprite->getPositionX() + 7.f);
    sprite->setScale(iconScale);
    sprite->setID("icon-sprite");
    bgSprite->addChild(sprite);

    auto label = CCSprite::create("labelSmall.png"_spr);
    label->setAnchorPoint({ 1, 0.5f });
    if (!Mod::get()->getSettingValue<bool>("ltr-texts")) {
		label->setPosition( { sprite->getPositionX() - sprite->getScaledContentWidth()/2 - 5.f, 40.f });
	} else {
        label->setScaleX(-1);
		label->setPosition( { 7.5f, 40.f });
	}
    label->setColor({ 0, 0, 0 });
    label->setOpacity(140);
	label->setID("smol-label");
	bgSprite->addChild(label);

    if (!CCMenuItemSpriteExtra::init(bgSprite, nullptr, target, callback)) return false;

    this->setID(id);
    this->m_scaleMultiplier = 1.1f;

    return true;
}

void MiniRDButton::rotateIcon(float rotation) {
	if (this->getID() != "search-button") {
		if (auto icon = this->getChildByIDRecursive("icon-sprite")) {
			icon->setRotation(rotation);
		}
	}
}

MiniRDButton* MiniRDButton::create(std::string iconSprite, float iconScale, ccColor3B color, CCObject* target, cocos2d::SEL_MenuHandler callback, std::string id) {
    auto ret = new MiniRDButton;

    if (ret && ret->init(iconSprite, iconScale, color, target, callback, id)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}