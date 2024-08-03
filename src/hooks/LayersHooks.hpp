#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../Variables.hpp"
using namespace geode::prelude;

#define getSelector(id, var) \
	if (auto button = creatorLayer->getChildByIDRecursive(id)) {\
		var = as<CCMenuItemSpriteExtra*>(button)->m_pfnSelector;\
	}

std::vector<int> mainLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 5001, 5002, 5003, 5004 };
int thing = 0;

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

class $modify(LoadingLayer) {
    void loadAssets() {
        LoadingLayer::loadAssets();
		thing++;
		auto loader = Loader::get();
        if (thing == 8) {
            auto creatorLayer = CreatorLayer::create(); // phantom layer rip

			// thanks minecraftify
			getSelector("cvolton.betterinfo/main-button", Variables::BISelector);
			getSelector("spaghettdev.gd-roulette/roulette-button", Variables::RouletteSelector);
			getSelector("super-expert-button", Variables::SupExSelector);
			getSelector("demon-progression-button", Variables::GDDPSelector);

            if (loader->isModLoaded("gdutilsdevs.gdutils")) {
                creatorLayer->retain();
            }
        }
    }
};

class $modify(CreatorLayer) {
    void onChallenge(CCObject* sender) {
        if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
            ChallengesPage::create()->show();
            return;
        }
        CreatorLayer::onChallenge(sender);
    }
};