#include "RDStatsNode.hpp"

bool RDStatsNode::init(std::string sprite, std::string string, std::string id) {
    if (!CCNode::init()) return false;

    auto icon = CCSprite::createWithSpriteFrameName(sprite.c_str());
    icon->setPosition({ 0, 2.f });
    icon->setScale(1.5f);
    icon->setID("icon-sprite");
    this->addChild(icon, 2);

    auto label = CCLabelBMFont::create(string.c_str(), "NewPusab.fnt"_spr);
    label->setScale(0.5f);
    label->setAnchorPoint({ 0, 0.5f });
    label->setPosition({ icon->getPositionX() + icon->getScaledContentWidth()/2 + 5.f, 7.5f });
    label->setID("stat-label");
    this->addChild(label, 1);

    auto bg0 = CCSprite::create("statsNodeHalf-2.png"_spr);
    bg0->setAnchorPoint({ 0, 0.5f });
    bg0->setScale(0.6f);
    bg0->setRotationY(180.f);
    bg0->setColor({ 0, 0, 0 });
    bg0->setOpacity(125);
    bg0->setPosition({ icon->getPositionX() + icon->getScaledContentWidth()/4, icon->getPositionY() });
    this->addChild(bg0, 0);

    if (sprite == "GJ_moonsIcon_001.png" || sprite == "GJ_diamondsIcon_001.png") {
        auto bg0b = CCSprite::create("statsNodeHalf-2.png"_spr);
        bg0b->setAnchorPoint({ 0, 0.5f });
        bg0b->setScale(0.6f);
        bg0b->setRotationX(180.f);
        bg0b->setColor({ 0, 0, 0 });
        bg0b->setOpacity(125);
        bg0b->setPosition({ bg0->getPositionX() - bg0->getScaledContentWidth(), bg0->getPositionY()});
        this->addChild(bg0b, 0);
    }

    auto bg1 = CCScale9Sprite::create("statsNodeHalf-1.png"_spr);
    bg1->setAnchorPoint({ 0, 0.5f });
    bg1->setScale(0.8f);
    bg1->setColor({ 0, 0, 0 });
    bg1->setOpacity(125);
    bg1->setContentSize({ icon->getScaledContentWidth() + label->getScaledContentWidth(), 30.f });
    bg1->setPosition(bg0->getPositionX(), bg0->getPositionY());
    this->addChild(bg1, 0);

    auto bg2 = CCSprite::create("statsNodeHalf-2.png"_spr);
    bg2->setAnchorPoint({ 0, 0.5f });
    bg2->setScale(0.6f);
    bg2->setColor({ 0, 0, 0 });
    bg2->setOpacity(125);
    bg2->setPosition({ bg1->getPositionX() + bg1->getScaledContentWidth(), bg1->getPositionY() });
    this->addChild(bg2, 0);

    this->setContentSize({ bg0->getScaledContentWidth() + bg1->getScaledContentWidth() + bg2->getScaledContentWidth(), 30.f});
    this->setPositionY(15.f);
    this->setID(id);
    this->setScale(0.9f);

    if (sprite == "GJ_moonsIcon_001.png") {
        icon->setScale(1.75f);
    }
    if (sprite == "GJ_demonIcon_001.png") {
        icon->setScale(1.4f);
    }
    if (sprite == "currencyDiamondIcon_001.png") {
        icon->setPositionX(3.5f);
    }
    return true;
}

RDStatsNode* RDStatsNode::create(std::string sprite, std::string string, std::string id) {
    auto ret = new RDStatsNode();
    if (ret && ret->init(sprite, string, id)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}