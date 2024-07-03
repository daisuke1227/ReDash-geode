#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>
using namespace geode::prelude;

// For some reason, click the back button on the GauntletSelectLayer (a.k.a the Gauntlet menu) 
// ALWAYS returns to CreatorLayer (a.k.a the menu after clicking the creators button on MenuLayer)
// So this hook just fixes that and returns us back to our modified MenuLayer

class $modify(GauntletSelectLayer) {
	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = MenuLayer::scene(false);
    	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};