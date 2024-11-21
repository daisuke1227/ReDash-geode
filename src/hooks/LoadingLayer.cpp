#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../Variables.hpp"
using namespace geode::prelude;

#define getSelector(id, var) \
	if (auto button = creatorLayer->getChildByIDRecursive(id)) {		\
		var = as<CCMenuItemSpriteExtra*>(button)->m_pfnSelector;		\
	}

int thing = 0;

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