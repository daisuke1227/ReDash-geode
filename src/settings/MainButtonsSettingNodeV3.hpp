#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

#include "MainButtonsSettingV3.hpp"

using namespace geode::prelude;

class MainButtonsSettingNodeV3 : public SettingNodeV3 {
protected:
    // both of these are arrays
    matjson::Value m_currentButtons;
    matjson::Value m_unselectedButtons;

    CCMenu* m_topMenu;
    CCMenu* m_bottomMenu;
    CCLabelBMFont* m_limitLabel;

    bool init(std::shared_ptr<MainButtonsSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;

    void updateIconRotation();
    void onAdd(CCObject* sender);
    void onRemove(CCObject* sender);

    void onCommit() override;
    void onResetToDefault() override;

public:
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    std::shared_ptr<MainButtonsSettingV3> getSetting() const;

    static MainButtonsSettingNodeV3* create(std::shared_ptr<MainButtonsSettingV3> setting, float width);
};