#include "RDButton.hpp"

bool RDButton::init(CCObject* target, std::string title, std::string description, std::string sprite, cocos2d::SEL_MenuHandler callback) {
    auto spriteNode = CCNode::create();

	auto buttonSpr1 = CCScale9Sprite::create("longButtonHalf-1.png"_spr);
	buttonSpr1->setContentSize({150.f, 35.f});
	buttonSpr1->setPosition(ccp(75.f, 52.f));
	spriteNode->addChild(buttonSpr1);

	auto buttonSpr2 = CCScale9Sprite::create("longButtonHalf-2.png"_spr);
	buttonSpr2->setContentSize({150.f, 35.f});
	buttonSpr2->setPosition(ccp(75.f, 17.f));
	spriteNode->addChild(buttonSpr2);

    auto icon = CCSprite::createWithSpriteFrameName(sprite.c_str());
    icon->setPosition(ccp(122, 34));
    icon->setScale(1.55f);
    spriteNode->addChild(icon);
	
	auto titleLabel = CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
	titleLabel->setScale(0.5f);
	titleLabel->setAnchorPoint(ccp(0, 0));
	titleLabel->setPosition(ccp(5, 50));
	spriteNode->addChild(titleLabel);

	auto descLabel = CCLabelBMFont::create(description.c_str(), "NewPusab.fnt"_spr, 225, CCTextAlignment::kCCTextAlignmentRight);
	descLabel->setPosition(ccp(icon->getPositionX() - icon->getScaledContentWidth()/2 - 3.f, 25));
	descLabel->setAnchorPoint(ccp(1,0));
	descLabel->setScale(0.4f);
	descLabel->setOpacity(150.f);
	descLabel->setColor(ccc3(0, 0, 0));
	spriteNode->addChild(descLabel);

    if (!CCMenuItemSpriteExtra::init(spriteNode, nullptr, target, callback)) return false;

	this->setContentSize({150.f, 70.f});
	this->m_scaleMultiplier = 1.1f;

	return true;
}

RDButton* RDButton::create(CCObject* target, std::string title, std::string description, std::string sprite, cocos2d::SEL_MenuHandler callback) {
    auto ret = new RDButton();
    if (ret && ret->init(target, title, description, sprite, callback)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}