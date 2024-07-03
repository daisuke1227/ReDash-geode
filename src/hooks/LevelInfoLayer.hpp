#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
using namespace geode::prelude;

class $modify(LevelInfoLayer) {
	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;
        log::error("{}", level->m_featured);
        log::error("{}", level->m_isEpic);
		log::info("{}", level->getAverageDifficulty());
        log::info("{} {}", level->m_demon.value(), level->m_demonDifficulty);
        return true;
	}
};