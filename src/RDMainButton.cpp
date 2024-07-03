#include "RDMainButton.hpp"

bool RDMainButton::init(CCPoint position, CCSize size, cocos2d::SEL_MenuHandler callback) {
    auto spriteNode = CCNode::create();

    if (!CCMenuItemSpriteExtra::init(spriteNode, nullptr, this, callback)) return false;

    auto background = CCScale9Sprite::create("GJ_square02.png");
    background->setContentSize(size);
    background->setPosition({ size.width/2, size.height/2 });
    spriteNode->addChild(background);

    auto crownSprite = CCSprite::createWithSpriteFrameName("RD_mainLevel.png"_spr);
    crownSprite->setScale(0.3f);
    crownSprite->setPosition({ size.width/2, size.height + 3.f });
    spriteNode->addChild(crownSprite);

    auto titleSprite = CCSprite::createWithSpriteFrameName("RD_mainLevelsLabel.png"_spr);
    titleSprite->setScale(0.58f);
    titleSprite->setPosition({ size.width/2, size.height - 22.5f });
    this->addChild(titleSprite);

    this->setContentSize(size);
    this->setPosition(position + size/2);
	this->m_scaleMultiplier = 1.1f;

    return true;
}

RDMainButton* RDMainButton::create(CCPoint position, CCSize size, cocos2d::SEL_MenuHandler callback) {
    auto ret = new RDMainButton();
    if (ret && ret->init(position, size, callback)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}