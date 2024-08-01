#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/modify/GauntletSelectLayer.hpp>
class $modify(GauntletSelectLayer) {
	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = MenuLayer::scene(false);
    	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};

#include <Geode/modify/SecretRewardsLayer.hpp>
class $modify(SecretRewardsLayer) {
	// crashing on android
	#ifndef GEODE_IS_ANDROID
	void keyBackClicked() {
		CCScene* sc = MenuLayer::scene(false);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
	#endif

	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = MenuLayer::scene(false);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};

#include <Geode/modify/SecretLayer2.hpp>
class $modify(SecretLayer2) {
	// same here
	#ifndef GEODE_IS_ANDROID
	void keyBackClicked() {
		CCScene* sc = MenuLayer::scene(false);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
	#endif

	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = MenuLayer::scene(false);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};