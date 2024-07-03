#include <Geode/Geode.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
using namespace geode::prelude;

class $modify(SecretRewardsLayer) {
	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = CCScene::create();
		sc->addChild(MenuLayer::scene(false));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};