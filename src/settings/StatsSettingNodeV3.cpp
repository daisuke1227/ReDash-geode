#include "MiniButtons.hpp"
#include "StatsSettingNodeV3.hpp"
using namespace geode::prelude;

bool StatsSettingNodeV3::init(std::shared_ptr<StatsSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) return false;

    this->m_currentStats = setting->getStats();
    this->m_unselectedStats = setting->getNonStats();

    float height = 100.f;
    this->setContentSize({ width, height });

    auto nameLabel = this->getNameLabel();
    nameLabel->setScale(0.4f);
    nameLabel->getParent()->setPosition({width / 2 + (nameLabel->getParent()->getScaledContentWidth() - nameLabel->getScaledContentWidth())/2, height - 10.f});
    nameLabel->getParent()->setAnchorPoint({0.5f, 0.5f});
    nameLabel->setID("name-label");

    auto topMenu = CCMenu::create();
    topMenu->setPosition({ width/2 , height - 35 });
    topMenu->setContentSize({ width - 30, 30 });
    topMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(5.f)
            ->setAutoScale(false)
    );
    topMenu->setID("top-menu");
    this->addChild(topMenu);
    m_topMenu = topMenu;

    auto floorLine = CCSprite::createWithSpriteFrameName("floorLine_01_001.png");
    floorLine->setPosition({ width/2 , (height - 10)/2 });
    floorLine->setScaleX((width - 30) / floorLine->getContentWidth());
    floorLine->setID("floor-line");
    this->addChild(floorLine);

    auto bottomMenu = CCMenu::create();
    bottomMenu->setPosition({ width/2 , 25 });
    bottomMenu->setContentSize({ width - 30, 30 });
    bottomMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(5.f)
            ->setAutoScale(false)
    );
    bottomMenu->setID("bottom-menu");
    this->addChild(bottomMenu);
    m_bottomMenu = bottomMenu;

    this->updateState(nullptr);
    return true;
}

void StatsSettingNodeV3::updateState(CCNode* invoker) {
    SettingNodeV3::updateState(invoker);

    if (invoker == nullptr) {
        m_topMenu->removeAllChildren();
        m_bottomMenu->removeAllChildren();

        for (auto& stat : m_currentStats) {
            auto id = stat.asString().unwrapOr("diamond-diamonds-stats");
            m_topMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 0, 255, 0 }, this, menu_selector(StatsSettingNodeV3::onRemove), id));
        }

        for (auto& stat : m_unselectedStats) {
            auto id = stat.asString().unwrapOr("diamond-diamonds-stats");
            m_bottomMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 255, 50, 0 }, this, menu_selector(StatsSettingNodeV3::onAdd), id));
        }
        
        m_topMenu->updateLayout();
        m_bottomMenu->updateLayout();
    } else {
        auto id = invoker->getID();

        if (m_topMenu->getChildByIDRecursive(id)) {
            invoker->removeFromParent();
            m_bottomMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 255, 50, 0 }, this, menu_selector(StatsSettingNodeV3::onAdd), id));;

            m_topMenu->updateLayout();
            m_bottomMenu->updateLayout();
        } else if (m_bottomMenu->getChildByIDRecursive(id)) {
            invoker->removeFromParent();
            m_topMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 0, 255, 0 }, this, menu_selector(StatsSettingNodeV3::onRemove), id));;

            m_topMenu->updateLayout();
            m_bottomMenu->updateLayout();
        }
    }
}

void StatsSettingNodeV3::onAdd(CCObject* sender) {
    auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto id = thisBtn->getID();

    m_currentStats.push(id);
    auto r = matjson::Value::array();
    for (auto& stat : m_unselectedStats) {
        if (stat.asString().unwrapOr("") != id) r.push(stat);
    }
    m_unselectedStats = r;

    this->markChanged(thisBtn);
}

void StatsSettingNodeV3::onRemove(CCObject* sender) {
    auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto id = thisBtn->getID();

    m_unselectedStats.push(id);
    auto r = matjson::Value::array();
    for (auto& stat : m_currentStats) {
        if (stat.asString().unwrapOr("") != id) r.push(stat);
    }
    m_currentStats = r;

    this->markChanged(thisBtn);
}

void StatsSettingNodeV3::onCommit() {
    this->getSetting()->setStats(m_currentStats);
}

void StatsSettingNodeV3::onResetToDefault() {
    m_currentStats = DEFAULT_STATS;
    m_unselectedStats = matjson::Value::array();
    auto array = m_currentStats.asArray().unwrap();
    for (auto& stat : ALL_STATS) {
        if (std::find(array.begin(), array.end(), stat.asString().unwrapOr("")) == array.end()){
            m_unselectedStats.push(stat);
        }
    }

    this->markChanged(nullptr);
}

bool StatsSettingNodeV3::hasUncommittedChanges() const {
    return m_currentStats != this->getSetting()->getStats();
}

bool StatsSettingNodeV3::hasNonDefaultValue() const {
    return m_currentStats != DEFAULT_STATS;
}

std::shared_ptr<StatsSettingV3> StatsSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<StatsSettingV3>(SettingNodeV3::getSetting());
}

StatsSettingNodeV3* StatsSettingNodeV3::create(std::shared_ptr<StatsSettingV3> setting, float width) {
    auto res = new StatsSettingNodeV3();
    if (res && res->init(setting, width)) {
        res->autorelease();
        return res;
    }
    CC_SAFE_DELETE(res);
    return nullptr;
}