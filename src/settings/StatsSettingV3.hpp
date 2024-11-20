#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

extern std::map<std::string, std::string> idToStatSpr;
extern matjson::Value DEFAULT_STATS;
extern matjson::Value ALL_STATS;

class StatsSettingV3 : public SettingV3 {
protected:
    matjson::Value m_stats = DEFAULT_STATS;
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

    matjson::Value getStats() const {
        return m_stats;
    }

    void setStats(matjson::Value const& stats) {
        m_stats = stats;
    }

    matjson::Value getNonStats() const {
        auto res = matjson::Value::array();

        if (!m_stats.isArray()) {
            log::error("m_stats is not an array!");
            return res;
        }
        auto array = m_stats.asArray().unwrap();
        
        for (auto& stat : ALL_STATS) {
            if (std::find(array.begin(), array.end(), stat.asString().unwrapOr("")) == array.end()) {
                res.push(stat);
            }
        }
        return res;
    }
};