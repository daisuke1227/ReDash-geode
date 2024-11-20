#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

#include "StatsSettingV3.hpp"

using namespace geode::prelude;

class StatsSettingNodeV3 : public SettingNodeV3 {
protected:
    // both of these are arrays
    matjson::Value m_currentStats;
    matjson::Value m_unselectedStats;

    CCMenu* m_topMenu;
    CCMenu* m_bottomMenu;

    bool init(std::shared_ptr<StatsSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;

    void onAdd(CCObject* sender);
    void onRemove(CCObject* sender);

    void onCommit() override;
    void onResetToDefault() override;
public:
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    std::shared_ptr<StatsSettingV3> getSetting() const;

    static StatsSettingNodeV3* create(std::shared_ptr<StatsSettingV3> setting, float width);
};