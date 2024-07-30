#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;

#define topButton(statSpr)\
    auto bgSprite = CCScale9Sprite::create("RD_square.png"_spr);\
    bgSprite->setScale(0.3f);\
\
    auto sprite = CCSprite::createWithSpriteFrameName(statStr.c_str());\
    sprite->setPosition(bgSprite->getContentSize() / 2);\
    sprite->setScale(2.f);\
    bgSprite->addChild(sprite);\
\
    bgSprite->setColor({ 0, 255, 0});\
    auto button = CCMenuItemSpriteExtra::create(\
        bgSprite,\
        this,\
        menu_selector(StatsSettingNode::onRemove)\
    );\
    button->setID(statStr);\
    m_topMenu->addChild(button);

#define bottomButton(statSpr)\
    auto bgSprite = CCScale9Sprite::create("RD_square.png"_spr);\
    bgSprite->setScale(0.3f);\
\
    auto sprite = CCSprite::createWithSpriteFrameName(statStr.c_str());\
    sprite->setPosition(bgSprite->getContentSize() / 2);\
    sprite->setScale(2.f);\
    bgSprite->addChild(sprite);\
\
    bgSprite->setColor({ 255, 50, 0});\
    auto button = CCMenuItemSpriteExtra::create(\
        bgSprite,\
        this,\
        menu_selector(StatsSettingNode::onAdd)\
    );\
    button->setID(statStr);\
    m_bottomMenu->addChild(button);


auto DEFAULT_ARRAY = matjson::parse(R"(["GJ_starsIcon_001.png", "GJ_moonsIcon_001.png", "GJ_coinsIcon_001.png", "GJ_coinsIcon2_001.png", "GJ_demonIcon_001.png"])").as_array();
auto ALL_STATS = matjson::parse(R"(["GJ_starsIcon_001.png", "GJ_moonsIcon_001.png", "GJ_coinsIcon_001.png", "GJ_coinsIcon2_001.png", "GJ_demonIcon_001.png", "GJ_diamondsIcon_001.png", "currencyDiamondIcon_001.png", "currencyOrbIcon_001.png"])").as_array();

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
        floorLine->setPosition({ width/2 , height/2 - 5});
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
            auto statStr = stat.as_string();
            topButton(statStr);
        }

        for (auto& stat : m_unselectedStatsArray) {
            auto statStr = stat.as_string();
            bottomButton(statStr);
        }
        
        topMenu->updateLayout();
        bottomMenu->updateLayout();

        return true;
    }

    void onAdd(CCObject* sender) {
        auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto statStr = thisBtn->getID();

        m_currentStatsArray.push_back(statStr);
        m_unselectedStatsArray.erase(std::remove(m_unselectedStatsArray.begin(), m_unselectedStatsArray.end(), statStr), m_unselectedStatsArray.end());

        thisBtn->removeFromParent();
        topButton(statStr);

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
        auto statStr = thisBtn->getID();

        m_unselectedStatsArray.push_back(statStr);
        m_currentStatsArray.erase(std::remove(m_currentStatsArray.begin(), m_currentStatsArray.end(), statStr), m_currentStatsArray.end());

        thisBtn->removeFromParent();
        bottomButton(statStr);

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
        return m_currentStatsArray != DEFAULT_ARRAY;
    }
    void resetToDefault() override {
        m_currentStatsArray = DEFAULT_ARRAY;
        m_unselectedStatsArray = matjson::Array();
        for (auto& stat : ALL_STATS) {
            if (std::find(m_currentStatsArray.begin(), m_currentStatsArray.end(), stat) == m_currentStatsArray.end()) {
                m_unselectedStatsArray.push_back(stat.as_string());
            }
        }
        m_topMenu->removeAllChildren();
        m_bottomMenu->removeAllChildren();

        for (auto& stat : m_currentStatsArray) {
            auto statStr = stat.as_string();
            topButton(statStr);
        }
        for (auto& stat : m_unselectedStatsArray) {
            auto statStr = stat.as_string();
            bottomButton(statStr);
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
    Mod::get()->addCustomSetting<StatsSettingValue>("stats-nodes-selection", DEFAULT_ARRAY);
}