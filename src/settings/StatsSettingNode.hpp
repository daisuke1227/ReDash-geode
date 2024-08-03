#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "MiniButtons.hpp"
using namespace geode::prelude;

std::map<std::string, std::string> idToStatSpr = {
    {"stars-stats", "GJ_starsIcon_001.png"},
    {"moons-stats", "GJ_moonsIcon_001.png"},
    {"secret-coins-stats", "GJ_coinsIcon_001.png"},
    {"user-coins-stats", "GJ_coinsIcon2_001.png"},
    {"demons-stats", "GJ_demonIcon_001.png"},
    {"diamonds-stats", "GJ_diamondsIcon_001.png"},
    {"diamond-diamonds-stats", "currencyDiamondIcon_001.png"},
    {"orbs-stats", "currencyOrbIcon_001.png"},
};

auto DEFAULT_STATS = matjson::parse(R"(["stars-stats", "moons-stats", "secret-coins-stats", "user-coins-stats", "demons-stats"])").as_array();
auto ALL_STATS = matjson::parse(R"(["stars-stats", "moons-stats", "secret-coins-stats", "user-coins-stats", "demons-stats", "diamonds-stats", "diamond-diamonds-stats", "orbs-stats"])").as_array();

class StatsSettingValue : public SettingValue {
protected:
    matjson::Array m_statsArray;

public:
    StatsSettingValue(std::string const& key, std::string const& modID, matjson::Array statsArray)
      : SettingValue(key, modID), m_statsArray(statsArray) {}

    bool load(matjson::Value const& json) override {
        if (!json.is_array()) return false;
        m_statsArray = json.as_array();
        return true;
    }
    bool save(matjson::Value& json) const override {
        json = m_statsArray;
        return true;
    }

    SettingNode* createNode(float width) override;

    void setStatsArray(matjson::Array statsArray) { m_statsArray = statsArray; }
    matjson::Array getStatsArray() const { return m_statsArray; }
};

class StatsSettingNode : public SettingNode {
protected:
    matjson::Array m_currentStatsArray;
    matjson::Array m_unselectedStatsArray;

    CCMenu* m_topMenu;
    CCMenu* m_bottomMenu;
    CCMenuItemSpriteExtra* m_resetButton;
    CCLabelBMFont* m_nameLabel;

    bool init(StatsSettingValue* value, float width) {
        if (!SettingNode::init(value))
            return false;

        m_currentStatsArray = value->getStatsArray();
        m_unselectedStatsArray = matjson::Array();
        for (auto& stat : ALL_STATS) {
            if (std::find(m_currentStatsArray.begin(), m_currentStatsArray.end(), stat) == m_currentStatsArray.end()) {
                m_unselectedStatsArray.push_back(stat.as_string());
            }
        }
        
        float height = 100.f;
        this->setContentSize({ width, height });

        auto menu = CCMenu::create();
        menu->setContentSize({ width, height });
        menu->setPosition({ width/2 , height/2 });
        menu->ignoreAnchorPointForPosition(false);
        menu->setID("label-menu");
        this->addChild(menu);

        auto label = CCLabelBMFont::create(
            Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("name").c_str(),
            "bigFont.fnt"
        );
        label->setPosition({ width / 2, height - 10 });
        label->setScale(.4f);
        label->setID("name-label");
        m_nameLabel = label;
        menu->addChild(label);

        auto resetSpr = CCSprite::createWithSpriteFrameName("geode.loader/reset-gold.png");
        resetSpr->setScale(.5f);
        auto resetButton = CCMenuItemSpriteExtra::create(
            resetSpr,
            this,
            menu_selector(StatsSettingNode::resetToDefault)
        );
        resetButton->setVisible(false);
        resetButton->setPosition({ label->getPositionX() + label->getScaledContentWidth()/2 + 5 + resetButton->getScaledContentWidth()/2 , height - 10 });
        resetButton->setID("reset-button");
        m_resetButton = resetButton;
        menu->addChild(resetButton);
        if (hasNonDefaultValue()) resetButton->setVisible(true);

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

        for (auto& stat : m_currentStatsArray) {
            auto id = stat.as_string();
            m_topMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 0, 255, 0 }, this, menu_selector(StatsSettingNode::onRemove), id));
        }

        for (auto& stat : m_unselectedStatsArray) {
            auto id = stat.as_string();
            m_bottomMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 255, 50, 0 }, this, menu_selector(StatsSettingNode::onAdd), id));
        }
        
        m_topMenu->updateLayout();
        m_bottomMenu->updateLayout();

        return true;
    }

    void onAdd(CCObject* sender) {
        auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto id = thisBtn->getID();

        m_currentStatsArray.push_back(id);
        m_unselectedStatsArray.erase(std::remove(m_unselectedStatsArray.begin(), m_unselectedStatsArray.end(), id), m_unselectedStatsArray.end());

        thisBtn->removeFromParent();
        m_topMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 0, 255, 0 }, this, menu_selector(StatsSettingNode::onRemove), id));;

        m_topMenu->updateLayout();
        m_bottomMenu->updateLayout();

        this->dispatchChanged();

        if (hasNonDefaultValue()) m_resetButton->setVisible(true);
        else m_resetButton->setVisible(false);
        if (hasUncommittedChanges()) m_nameLabel->setColor({ 17, 221, 0});
        else m_nameLabel->setColor({ 255, 255, 255});
    }

    void onRemove(CCObject* sender) {
        auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto id = thisBtn->getID();

        m_unselectedStatsArray.push_back(id);
        m_currentStatsArray.erase(std::remove(m_currentStatsArray.begin(), m_currentStatsArray.end(), id), m_currentStatsArray.end());

        thisBtn->removeFromParent();
        m_bottomMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 255, 50, 0 }, this, menu_selector(StatsSettingNode::onAdd), id));;

        m_topMenu->updateLayout();
        m_bottomMenu->updateLayout();

        this->dispatchChanged();

        if (hasNonDefaultValue()) m_resetButton->setVisible(true);
        else m_resetButton->setVisible(false);
        if (hasUncommittedChanges()) m_nameLabel->setColor({ 17, 221, 0});
        else m_nameLabel->setColor({ 255, 255, 255});
    }

public:
    void commit() override {
        static_cast<StatsSettingValue*>(m_value)->setStatsArray(m_currentStatsArray);
        this->dispatchCommitted();
        m_nameLabel->setColor({ 255, 255, 255});
    }
    bool hasUncommittedChanges() override {
        return m_currentStatsArray != static_cast<StatsSettingValue*>(m_value)->getStatsArray();
    }
    bool hasNonDefaultValue() override {
        return m_currentStatsArray != DEFAULT_STATS;
    }
    void resetToDefault() override {
        m_currentStatsArray = DEFAULT_STATS;
        m_unselectedStatsArray = matjson::Array();
        for (auto& stat : ALL_STATS) {
            if (std::find(m_currentStatsArray.begin(), m_currentStatsArray.end(), stat) == m_currentStatsArray.end()) {
                m_unselectedStatsArray.push_back(stat.as_string());
            }
        }
        m_topMenu->removeAllChildren();
        m_bottomMenu->removeAllChildren();

        for (auto& stat : m_currentStatsArray) {
            auto id = stat.as_string();
            m_topMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 0, 255, 0 }, this, menu_selector(StatsSettingNode::onRemove), id));
        }

        for (auto& stat : m_unselectedStatsArray) {
            auto id = stat.as_string();
            m_bottomMenu->addChild(MiniButton::create(idToStatSpr[id], 2.f, { 255, 50, 0 }, this, menu_selector(StatsSettingNode::onAdd), id));
        }

        m_topMenu->updateLayout();
        m_bottomMenu->updateLayout();

        m_resetButton->setVisible(false);
        if (hasUncommittedChanges()) m_nameLabel->setColor({ 17, 221, 0});
        else m_nameLabel->setColor({ 255, 255, 255});

        this->dispatchChanged();
    }

    static StatsSettingNode* create(StatsSettingValue* value, float width) {
        auto ret = new StatsSettingNode;
        if (ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

SettingNode* StatsSettingValue::createNode(float width) {
    return StatsSettingNode::create(this, width);
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<StatsSettingValue>("stats-nodes-selection", DEFAULT_STATS);

    auto statsSettings = as<StatsSettingValue*>(Mod::get()->getSetting("stats-nodes-selection"));
    std::vector<std::string> oldStats = {"GJ_starsIcon_001.png", "GJ_moonsIcon_001.png", "GJ_coinsIcon_001.png", "GJ_coinsIcon2_001.png", "GJ_demonIcon_001.png", "GJ_diamondsIcon_001.png", "currencyDiamondIcon_001.png", "currencyOrbIcon_001.png"};
    for (auto& stat : statsSettings->getStatsArray()) {
        log::info("yippee '{}'", stat.as_string());
        if (find(oldStats.begin(), oldStats.end(), stat.as_string()) != oldStats.end()){
            statsSettings->setStatsArray(DEFAULT_STATS);
            break;
        }

    }
}