#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
using namespace geode::prelude;

#define topMainButton(id, scale)\
    auto buttonStr = idToButtonSpr[button.as_string()].first;\
\
    auto bgSprite = CCScale9Sprite::create("RD_square.png"_spr);\
    bgSprite->setContentWidth(bgSprite->getContentWidth() / 80 * 150);\
    bgSprite->setScale(0.3f);\
\
    auto sprite = CCSprite::createWithSpriteFrameName(buttonStr.c_str());\
    sprite->setPosition(bgSprite->getContentSize()/2 + ccp(bgSprite->getContentWidth()/2 - sprite->getContentWidth()/2 - 0.5f, 0));\
    sprite->setScale(scale);\
    sprite->setID("icon-sprite");\
    bgSprite->addChild(sprite);\
\
    bgSprite->setColor({ 0, 255, 0});\
    auto button = CCMenuItemSpriteExtra::create(\
        bgSprite,\
        this,\
        menu_selector(MainButtonsSettingNode::onRemove)\
    );\
\
    auto label = CCSprite::create("labelSmall.png"_spr);\
    label->setAnchorPoint({ 1, 0.5f });\
    if (!Mod::get()->getSettingValue<bool>("ltr-texts")) {\
		label->setPosition( { sprite->getPositionX() - sprite->getScaledContentWidth()/2 - 3.f, 40.f });\
	} else {\
        label->setScaleX(-1);\
		label->setPosition( { 7.5f, 40.f });\
	}\
	label->setID("smol-label");\
	bgSprite->addChild(label);\
\
    bgSprite->setID("bg-sprite");\
    button->setID(id);\
    button->m_scaleMultiplier = 1.1f;\
    m_topMenu->addChild(button);


#define bottomMainButton(id, scale)\
    auto buttonStr = idToButtonSpr[button.as_string()].first;\
\
    auto bgSprite = CCScale9Sprite::create("RD_square.png"_spr);\
    bgSprite->setScale(0.3f);\
\
    auto sprite = CCSprite::createWithSpriteFrameName(buttonStr.c_str());\
    sprite->setPosition(bgSprite->getContentSize() / 2);\
    sprite->setScale(scale);\
    sprite->setID("icon-sprite");\
    bgSprite->addChild(sprite);\
\
    bgSprite->setColor({ 255, 50, 0});\
    auto button = CCMenuItemSpriteExtra::create(\
        bgSprite,\
        this,\
        menu_selector(MainButtonsSettingNode::onAdd)\
    );\
    bgSprite->setID("bg-sprite");\
    button->setID(id);\
    m_bottomMenu->addChild(button);

std::map<std::string, std::pair<std::string, float>> idToButtonSpr = {
    {"create-button", {"RD_create.png"_spr, 0.95f}},
    {"saved-button", {"RD_saved.png"_spr, 0.95f}},
    {"paths-button", {"RD_paths.png"_spr, 0.8f}},
    {"leaderboards-button", {"RD_leaderboards.png"_spr, 0.85f}},
    {"gauntlets-button", {"RD_gauntlets.png"_spr, 1.f}},
    {"featured-button", {"RD_featured.png"_spr, 0.95f}},    
    {"lists-button", {"RD_lists.png"_spr, 1.f}},
    {"search-button", {"RD_search.png"_spr, 0.9f}},
    {"mappacks-button", {"RD_mappacks.png"_spr, 0.75f}},
    {"quests-button", {"RD_quests.png"_spr, 1.f}},
    {"the-map-button", {"RD_map.png"_spr, 0.95f}},
    {"versus-button", {"RD_versus.png"_spr, 0.95f}}
};

auto DEFAULT_BUTTONS = matjson::parse(R"(["create-button", "saved-button", "paths-button", "leaderboards-button", "gauntlets-button", "featured-button", "lists-button", "search-button"])").as_array();
auto ALL_BUTTONS = matjson::parse(R"(["create-button", "saved-button", "paths-button", "leaderboards-button", "gauntlets-button", "featured-button", "lists-button", "search-button", "mappacks-button", "quests-button", "the-map-button", "versus-button"])").as_array();

class MainButtonsSettingValue : public SettingValue {
protected:
    matjson::Array m_buttonsArray;

public:
    MainButtonsSettingValue(std::string const& key, std::string const& modID, matjson::Array buttonsArray)
      : SettingValue(key, modID), m_buttonsArray(buttonsArray) {}

    bool load(matjson::Value const& json) override {
        if (!json.is_array()) return false;
        m_buttonsArray = json.as_array();
        return true;
    }
    bool save(matjson::Value& json) const override {
        json = m_buttonsArray;
        return true;
    }

    SettingNode* createNode(float width) override;

    void setButtonsArray(matjson::Array buttonsArray) { m_buttonsArray = buttonsArray; }
    matjson::Array getButtonsArray() const { return m_buttonsArray; }
};

class MainButtonsSettingNode : public SettingNode {
protected:
    matjson::Array m_currentButtonsArray;
    matjson::Array m_unselectedButtonsArray;

    CCMenu* m_topMenu;
    CCMenu* m_bottomMenu;
    CCMenuItemSpriteExtra* m_resetButton;
    CCLabelBMFont* m_nameLabel;

    bool init(MainButtonsSettingValue* value, float width) {
        if (!SettingNode::init(value))
            return false;

        m_currentButtonsArray = value->getButtonsArray();
        m_unselectedButtonsArray = matjson::Array();
        for (auto& button : ALL_BUTTONS) {
            if (std::find(m_currentButtonsArray.begin(), m_currentButtonsArray.end(), button) == m_currentButtonsArray.end()) {
                m_unselectedButtonsArray.push_back(button.as_string());
            }
        }
        
        float height = 130.f;
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
            menu_selector(MainButtonsSettingNode::resetToDefault)
        );
        resetButton->setVisible(false);
        resetButton->setPosition({ label->getPositionX() + label->getScaledContentWidth()/2 + 5 + resetButton->getScaledContentWidth()/2 , height - 10 });
        resetButton->setID("reset-button");
        m_resetButton = resetButton;
        menu->addChild(resetButton);
        if (hasNonDefaultValue()) resetButton->setVisible(true);

        auto topMenu = CCMenu::create();
        topMenu->setPosition({ width/2 , height - 50 });
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
                ->setAxisAlignment(AxisAlignment::Start)
                ->setGap(5.f)
                ->setAutoScale(false)
        );
        bottomMenu->setID("bottom-menu");
        this->addChild(bottomMenu);
        m_bottomMenu = bottomMenu;

        for (auto& button : m_currentButtonsArray) {
            auto id = button.as_string();
            auto scale = idToButtonSpr[id].second;
            topMainButton(id, scale);
        }

        for (auto& button : m_unselectedButtonsArray) {
            auto id = button.as_string();
            auto scale = idToButtonSpr[id].second;
            bottomMainButton(id, scale);
        }
        
        topMenu->updateLayout();
        bottomMenu->updateLayout();
        updateIconRotation();

        return true;
    }

    void updateIconRotation() {
        if (Mod::get()->getSettingValue<bool>("rotate-buttons-logos")) {
			for (auto& button: CCArrayExt<CCMenuItemSpriteExtra*>(m_topMenu->getChildren())) {
                if (button->getID() != "search-button"_spr) {
                    if (button->getPositionX() < m_topMenu->getContentWidth()/2) {
                        button->getChildByIDRecursive("icon-sprite")->setRotation(10.f);
                    } else if (button->getPositionX() > m_topMenu->getContentWidth()/2) {
                        button->getChildByIDRecursive("icon-sprite")->setRotation(-10.f);
                    }
                }
			}
		}
    }

    void onAdd(CCObject* sender) {
    //     auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    //     auto buttonStr = thisBtn->getID();

    //     m_currentButtonsArray.push_back(buttonStr);
    //     m_unselectedButtonsArray.erase(std::remove(m_unselectedButtonsArray.begin(), m_unselectedButtonsArray.end(), buttonStr), m_unselectedButtonsArray.end());

    //     thisBtn->removeFromParent();
    //     topMainButton(buttonStr);

    //     m_topMenu->updateLayout();
    //     m_bottomMenu->updateLayout();

    //     this->dispatchChanged();

    //     if (hasNonDefaultValue()) m_resetButton->setVisible(true);
    //     else m_resetButton->setVisible(false);
    //     if (hasUncommittedChanges()) m_nameLabel->setColor({ 17, 221, 0});
    //     else m_nameLabel->setColor({ 255, 255, 255});
    }

    void onRemove(CCObject* sender) {
    //     auto thisBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
    //     auto buttonStr = thisBtn->getID();

    //     m_unselectedButtonsArray.push_back(buttonStr);
    //     m_currentButtonsArray.erase(std::remove(m_currentButtonsArray.begin(), m_currentButtonsArray.end(), buttonStr), m_currentButtonsArray.end());

    //     thisBtn->removeFromParent();
    //     bottomMainButton(buttonStr);

    //     m_topMenu->updateLayout();
    //     m_bottomMenu->updateLayout();

    //     this->dispatchChanged();

    //     if (hasNonDefaultValue()) m_resetButton->setVisible(true);
    //     else m_resetButton->setVisible(false);
    //     if (hasUncommittedChanges()) m_nameLabel->setColor({ 17, 221, 0});
    //     else m_nameLabel->setColor({ 255, 255, 255});
    }

public:
    void commit() override {
        static_cast<MainButtonsSettingValue*>(m_value)->setButtonsArray(m_currentButtonsArray);
        this->dispatchCommitted();
        m_nameLabel->setColor({ 255, 255, 255});
    }
    bool hasUncommittedChanges() override {
        return m_currentButtonsArray != static_cast<MainButtonsSettingValue*>(m_value)->getButtonsArray();
        return false;
    }
    bool hasNonDefaultValue() override {
        return m_currentButtonsArray != DEFAULT_BUTTONS;
        return false;
    }
    void resetToDefault() override {
        m_currentButtonsArray = DEFAULT_BUTTONS;
        m_unselectedButtonsArray = matjson::Array();
        for (auto& button : ALL_BUTTONS) {
            if (std::find(m_currentButtonsArray.begin(), m_currentButtonsArray.end(), button) == m_currentButtonsArray.end()) {
                m_unselectedButtonsArray.push_back(button.as_string());
            }
        }
        // m_topMenu->removeAllChildren();
        // m_bottomMenu->removeAllChildren();

        // // for (auto& button : m_currentButtonsArray) {
        // //     auto buttonStr = button.as_string();
        // //     topMainButton(buttonStr);
        // // }
        // // for (auto& button : m_unselectedButtonsArray) {
        // //     auto buttonStr = button.as_string();
        // //     bottomMainButton(buttonStr);
        // // }

        // m_topMenu->updateLayout();
        // m_bottomMenu->updateLayout();

        // m_resetButton->setVisible(false);
        // if (hasUncommittedChanges()) m_nameLabel->setColor({ 17, 221, 0});
        // else m_nameLabel->setColor({ 255, 255, 255});

        // this->dispatchChanged();
    }

    static MainButtonsSettingNode* create(MainButtonsSettingValue* value, float width) {
        auto ret = new MainButtonsSettingNode;
        if (ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

SettingNode* MainButtonsSettingValue::createNode(float width) {
    return MainButtonsSettingNode::create(this, width);
}

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<MainButtonsSettingValue>("main-buttons-selection", DEFAULT_BUTTONS);
}