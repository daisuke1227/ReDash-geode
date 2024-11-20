#include "StatsSettingV3.hpp"
#include "StatsSettingNodeV3.hpp"
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

matjson::Value DEFAULT_STATS = matjson::parse(R"(["stars-stats", "moons-stats", "secret-coins-stats", "user-coins-stats", "demons-stats"])").unwrapOrDefault();
matjson::Value ALL_STATS = matjson::parse(R"(["stars-stats", "moons-stats", "secret-coins-stats", "user-coins-stats", "demons-stats", "diamonds-stats", "diamond-diamonds-stats", "orbs-stats"])").unwrapOrDefault();


Result<std::shared_ptr<SettingV3>> StatsSettingV3::parse(
    std::string const& key,
    std::string const& modID,
    matjson::Value const& json
) {
    auto res = std::make_shared<StatsSettingV3>();
    auto root = checkJson(json, "StatsSettingV3");

    res->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();
    
    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

bool StatsSettingV3::load(const matjson::Value& json) {
    this->m_stats = json.isArray() ? json : DEFAULT_STATS;

    // old stats id check
    std::vector<std::string> oldStats = {"GJ_starsIcon_001.png", "GJ_moonsIcon_001.png", "GJ_coinsIcon_001.png", "GJ_coinsIcon2_001.png", "GJ_demonIcon_001.png", "GJ_diamondsIcon_001.png", "currencyDiamondIcon_001.png", "currencyOrbIcon_001.png"};
    for (auto& stat : this->m_stats) {
        if (find(oldStats.begin(), oldStats.end(), stat.asString().unwrapOr("")) != oldStats.end()){
            this->m_stats = DEFAULT_STATS;
            break;
        }
    }

    return true;
}

bool StatsSettingV3::save(matjson::Value& json) const {
    json = this->m_stats;
    return true;
}

SettingNodeV3* StatsSettingV3::createNode(float width) {
    return StatsSettingNodeV3::create(
        std::static_pointer_cast<StatsSettingV3>(shared_from_this()),
        width
    );
}

bool StatsSettingV3::isDefaultValue() const {
    return this->m_stats == DEFAULT_STATS;
}

void StatsSettingV3::reset() {
    this->m_stats = DEFAULT_STATS;
    markChanged();
}

$execute {
    (void) Mod::get()->registerCustomSettingType("stats-nodes-selection", &StatsSettingV3::parse);
}