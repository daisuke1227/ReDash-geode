#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

extern std::map<std::string, std::pair<std::string, float>> idToButtonSpr;
extern matjson::Value DEFAULT_BUTTONS;
extern matjson::Value ALL_BUTTONS;

class MainButtonsSettingV3 : public SettingV3 {
protected:
    matjson::Value m_buttons = DEFAULT_BUTTONS;
public:
    static Result<std::shared_ptr<SettingV3>> parse(
        std::string const& key,
        std::string const& modID,
        matjson::Value const& json
    );

    bool load(const matjson::Value& json) override;
    bool save(matjson::Value& json) const override;
    SettingNodeV3* createNode(float width) override;
    bool isDefaultValue() const override;
    void reset() override;

    matjson::Value getButtons() const {
        return m_buttons;
    }
    void setButtons(matjson::Value const& buttons) {
        m_buttons = buttons;
    }
    matjson::Value getNonButtons() const {
        auto res = matjson::Value::array();

        if (!m_buttons.isArray()) {
            log::error("m_buttons is not an array!");
            return res;
        }
        auto array = m_buttons.asArray().unwrap();
        
        for (auto& button : ALL_BUTTONS) {
            if (std::find(array.begin(), array.end(), button.asString().unwrapOr("")) == array.end()) {
                res.push(button);
            }
        }
        return res;
    }
};