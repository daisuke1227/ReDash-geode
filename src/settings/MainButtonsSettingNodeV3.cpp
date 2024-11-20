#include "MiniButtons.hpp"
#include "MainButtonsSettingNodeV3.hpp"
using namespace geode::prelude;

bool MainButtonsSettingNodeV3::init(std::shared_ptr<MainButtonsSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) return false;

    this->m_currentButtons = setting->getButtons();
    this->m_unselectedButtons = setting->getNonButtons();

    float height = 130.f;
    this->setContentSize({ width, height });

    auto nameLabel = this->getNameLabel();
    nameLabel->setScale(0.4f);
    nameLabel->getParent()->setPosition({width / 2 + (nameLabel->getParent()->getScaledContentWidth() - nameLabel->getScaledContentWidth())/2, height - 10.f});
    nameLabel->getParent()->setAnchorPoint({0.5f, 0.5f});
    nameLabel->setID("name-label");

    auto limitLabel = CCLabelBMFont::create(
        "You can only have at most 8 buttons selected at a time.",
        "bigFont.fnt"
    );
    limitLabel->setPosition({ width / 2, height - 22 });
    limitLabel->setScale(0.25f);
    limitLabel->setColor({ 255, 0, 0 });
    limitLabel->setOpacity(0);
    limitLabel->setID("limit-label");
    this->addChild(limitLabel);
    this->m_limitLabel = limitLabel;

    auto topMenu = CCMenu::create();
    topMenu->setPosition({ width/2 , height - 55 });
    topMenu->setContentSize({ width - 120, 70 });
    topMenu->setLayout(
        RowLayout::create()
            ->setGap(5.f)
            ->setAutoScale(false)
            ->setGrowCrossAxis(true)
    );
    topMenu->setID("top-menu");
    this->addChild(topMenu);
    m_topMenu = topMenu;

    auto floorLine = CCSprite::createWithSpriteFrameName("floorLine_01_001.png");
    floorLine->setPosition({ width/2 , (height - 10)/3 });
    floorLine->setScaleX((width - 30) / floorLine->getContentWidth());
    floorLine->setID("floor-line");
    this->addChild(floorLine);

    auto bottomMenu = CCMenu::create();
    bottomMenu->setPosition({ width/2 , 20 });
    bottomMenu->setContentSize({ width - 30, 30 });
    bottomMenu->setLayout(
        RowLayout::create()
            ->setGap(5.f)
    );
    bottomMenu->setID("bottom-menu");
    this->addChild(bottomMenu);
    m_bottomMenu = bottomMenu;

    this->updateState(nullptr);
    return true;
}

void MainButtonsSettingNodeV3::updateState(CCNode* invoker) {
    SettingNodeV3::updateState(invoker);

    // go on...
    if (invoker == nullptr) {
        m_topMenu->removeAllChildren();
        m_bottomMenu->removeAllChildren();

        for (auto& button : m_currentButtons) {
            auto id = button.asString().unwrapOr("versus-button"); // lol idk
            auto spr = idToButtonSpr[id].first;
            auto scale = idToButtonSpr[id].second;
            m_topMenu->addChild(MiniRDButton::create(spr, scale, { 0, 255, 0 }, this, menu_selector(MainButtonsSettingNodeV3::onRemove), id));
        }

        for (auto& button : m_unselectedButtons) {
            auto id = button.asString().unwrapOr("versus-button");
            auto spr = idToButtonSpr[id].first;
            auto scale = idToButtonSpr[id].second;
            m_bottomMenu->addChild(MiniButton::create(spr, scale, { 255, 50, 0 }, this, menu_selector(MainButtonsSettingNodeV3::onAdd), id));
        }

        m_topMenu->updateLayout();
        m_bottomMenu->updateLayout();
        this->updateIconRotation();
    } else {
        auto id = invoker->getID();
        auto spr = idToButtonSpr[id].first;
        auto scale = idToButtonSpr[id].second;

        if (m_topMenu->getChildByIDRecursive(id)) {
            invoker->removeFromParent();
            m_bottomMenu->addChild(MiniButton::create(spr, scale, { 255, 50, 0 }, this, menu_selector(MainButtonsSettingNodeV3::onAdd), id));

            m_topMenu->updateLayout();
            m_bottomMenu->updateLayout();
            this->updateIconRotation();
        } else if (m_bottomMenu->getChildByIDRecursive(id)) {
            invoker->removeFromParent();
            m_topMenu->addChild(MiniRDButton::create(spr, scale, { 0, 255, 0 }, this, menu_selector(MainButtonsSettingNodeV3::onRemove), id));

            m_topMenu->updateLayout();
            m_bottomMenu->updateLayout();
            this->updateIconRotation();
        }
    }
}

void MainButtonsSettingNodeV3::updateIconRotation() {
    if (Mod::get()->getSettingValue<bool>("rotate-buttons-logos")) {
        for (auto& button: CCArrayExt<CCMenuItemSpriteExtra*>(m_topMenu->getChildren())) {
            if (button->getID() != "search-button") {
                if (button->getPositionX() < m_topMenu->getContentWidth()/2) {
                    button->getChildByIDRecursive("icon-sprite")->setRotation(10.f);
                } else if (button->getPositionX() > m_topMenu->getContentWidth()/2) {
                    button->getChildByIDRecursive("icon-sprite")->setRotation(-10.f);
                } else {
                    button->getChildByIDRecursive("icon-sprite")->setRotation(0.f);
                }
            }
        }
    }
}

void MainButtonsSettingNodeV3::onAdd(CCObject* sender) {
    auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto nameLabel = this->getNameLabel();
    auto id = thisBtn->getID();

    if (m_currentButtons.size() >= 8) {
        m_limitLabel->setOpacity(255);
        m_limitLabel->runAction(CCFadeOut::create(1.f));
        
        return;
    }
    m_currentButtons.push(id);
    // bro PLEASE
    auto r = matjson::Value::array();
    for (auto& button : m_unselectedButtons) {
        if (button.asString().unwrapOr("") != id) r.push(button);
    }
    m_unselectedButtons = r;

    this->markChanged(thisBtn);
}

void MainButtonsSettingNodeV3::onRemove(CCObject* sender) {
    auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto nameLabel = this->getNameLabel();
    auto id = thisBtn->getID();

    m_unselectedButtons.push(id);
    auto r = matjson::Value::array();
    for (auto& button : m_currentButtons) {
        if (button.asString().unwrapOr("") != id) r.push(button);
    }
    m_currentButtons = r;

    this->markChanged(thisBtn);
}

void MainButtonsSettingNodeV3::onCommit() {
    this->getSetting()->setButtons(m_currentButtons);
}

void MainButtonsSettingNodeV3::onResetToDefault() {
    m_currentButtons = DEFAULT_BUTTONS;
    m_unselectedButtons = matjson::Value::array();
    auto array = m_currentButtons.asArray().unwrap();
    for (auto& button : ALL_BUTTONS) {
        if (std::find(array.begin(), array.end(), button.asString().unwrapOr("")) == array.end()){
            m_unselectedButtons.push(button);
        }
    }

    this->markChanged(nullptr);
}

bool MainButtonsSettingNodeV3::hasUncommittedChanges() const {
    return m_currentButtons != this->getSetting()->getButtons();
}

bool MainButtonsSettingNodeV3::hasNonDefaultValue() const {
    return m_currentButtons != DEFAULT_BUTTONS;
}

std::shared_ptr<MainButtonsSettingV3> MainButtonsSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<MainButtonsSettingV3>(SettingNodeV3::getSetting());
}

MainButtonsSettingNodeV3* MainButtonsSettingNodeV3::create(std::shared_ptr<MainButtonsSettingV3> setting, float width) {
    auto res = new MainButtonsSettingNodeV3();
    if (res && res->init(setting, width)) {
        res->autorelease();
        return res;
    }
    CC_SAFE_DELETE(res);
    return nullptr;
}