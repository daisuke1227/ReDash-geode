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
	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = MenuLayer::scene(false);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};

#include <Geode/modify/SecretLayer2.hpp>
class $modify(SecretLayer2) {
	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = MenuLayer::scene(false);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};