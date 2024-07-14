#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../Variables.hpp"
using namespace geode::prelude;

std::vector<int> mainLevels = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 5001, 5002, 5003, 5004 };
int thing = 1;

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
		auto loader = Loader::get();
        thing++;
        if (thing == 15) {
            auto creatorLayer = CreatorLayer::create(); // phantom layer rip
			
			if (loader->isModLoaded("cvolton.betterinfo")) {
				if (auto menu = creatorLayer->getChildByID("cvolton.betterinfo/center-right-menu")) {
					if (auto button = menu->getChildByID("cvolton.betterinfo/main-button")) {
						Variables::BISelector = as<CCMenuItemSpriteExtra*>(button)->m_pfnSelector;
					}
				}
			}


			auto pagesMod = loader->getLoadedMod("alphalaneous.pages_api");
			if (pagesMod->getSettingValue<bool>("creator-layer-menu")) {
				if (auto pageMenu = creatorLayer->getChildByID("paged-creator-buttons-menu")) {
					if (auto node = pageMenu->getChildByID("pages")) {
						CCArrayExt<CCMenu*> pages = node->getChildren();
						for (auto& page : pages) {
							CCArrayExt<CCNode*> buttons = page->getChildren();
							for (auto& button : buttons) {
								if (button->getID() == "super-expert-button" && loader->isModLoaded("xanii.super_expert")) {
									Variables::SupExSelector = as<CCMenuItemSpriteExtra*>(button)->m_pfnSelector;
								} else if (button->getID() == "demon-progression-button" && loader->isModLoaded("minemaker0430.gddp_integration")) {
									Variables::GDDPSelector = as<CCMenuItemSpriteExtra*>(button)->m_pfnSelector;
								}
							}
						}
					}
				}
			} else {
				if (auto menu = creatorLayer->getChildByID("creator-buttons-menu")) {
					if (loader->isModLoaded("xanii.super_expert")) {
						if (auto button = menu->getChildByID("super-expert-button")) {
							Variables::SupExSelector = as<CCMenuItemSpriteExtra*>(button)->m_pfnSelector;
						}
					}
					if (loader->isModLoaded("minemaker0430.gddp_integration")) {
						if (auto button = menu->getChildByID("demon-progression-button")) {
							Variables::GDDPSelector = as<CCMenuItemSpriteExtra*>(button)->m_pfnSelector;
						}
					}
				}
			}

			creatorLayer->release();
        }
    }
};