#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

std::vector<int> mainLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 5001, 5002, 5003, 5004 };

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!this->PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        
        auto id = level->m_levelID.value();
        if (std::find(mainLevels.begin(), mainLevels.end(), id) != mainLevels.end()) {
            Mod::get()->setSavedValue<int64_t>("last-main-level", id);
        }

        return true;
    }
};