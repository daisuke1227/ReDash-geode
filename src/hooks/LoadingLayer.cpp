#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Macros.hpp>

#include "../Variables.hpp"
using namespace geode::prelude;

int thing = 0;

class $modify(LoadingLayer) {
    void loadAssets() {
        LoadingLayer::loadAssets();
		thing++;
		auto loader = Loader::get();
        if (thing == 8) {
            auto creatorLayer = CreatorLayer::create(); // phantom layer rip

			// thanks minecraftify
			RD_GET_SELECTOR("cvolton.betterinfo/main-button", Variables::BISelector);
			RD_GET_SELECTOR("spaghettdev.gd-roulette/roulette-button", Variables::RouletteSelector);
			RD_GET_SELECTOR("super-expert-button", Variables::SupExSelector);
			RD_GET_SELECTOR("demon-progression-button", Variables::GDDPSelector);

            if (loader->isModLoaded("gdutilsdevs.gdutils")) {
                creatorLayer->retain();
            }
        }
    }
};