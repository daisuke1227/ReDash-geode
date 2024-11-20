#include "MainButtonsSettingV3.hpp"
#include "MainButtonsSettingNodeV3.hpp"
using namespace geode::prelude;

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

matjson::Value DEFAULT_BUTTONS = matjson::parse(R"(["create-button", "saved-button", "paths-button", "leaderboards-button", "gauntlets-button", "featured-button", "lists-button", "search-button"])").unwrapOrDefault();
matjson::Value ALL_BUTTONS = matjson::parse(R"(["create-button", "saved-button", "paths-button", "leaderboards-button", "gauntlets-button", "featured-button", "lists-button", "search-button", "mappacks-button", "quests-button", "the-map-button", "versus-button"])").unwrapOrDefault();


Result<std::shared_ptr<SettingV3>> MainButtonsSettingV3::parse(
    std::string const& key,
    std::string const& modID,
    matjson::Value const& json
) {
    auto res = std::make_shared<MainButtonsSettingV3>();
    auto root = checkJson(json, "MainButtonsSettingV3");

    res->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();
    
    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

bool MainButtonsSettingV3::load(const matjson::Value& json) {
    this->m_buttons = json.isArray() ? json : DEFAULT_BUTTONS;
    return true;
}

bool MainButtonsSettingV3::save(matjson::Value& json) const {
    json = this->m_buttons;
    return true;
}

SettingNodeV3* MainButtonsSettingV3::createNode(float width) {
    return MainButtonsSettingNodeV3::create(
        std::static_pointer_cast<MainButtonsSettingV3>(shared_from_this()),
        width
    );
}

bool MainButtonsSettingV3::isDefaultValue() const {
    return this->m_buttons == DEFAULT_BUTTONS;
}

void MainButtonsSettingV3::reset() {
    this->m_buttons = DEFAULT_BUTTONS;
    markChanged();
}

$execute {
    (void) Mod::get()->registerCustomSettingType("main-buttons-selection", &MainButtonsSettingV3::parse);
}