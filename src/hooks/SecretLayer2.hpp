#include <Geode/Geode.hpp>
#include <Geode/modify/SecretLayer2.hpp>
using namespace geode::prelude;

// my fucking fault
void SecretLayer2_onBack(SecretLayer2* self, cocos2d::CCObject* sender) {
    CCScene* sc = CCScene::create();
    sc->addChild(MenuLayer::scene(false));
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(geode::addresser::getNonVirtual(&SecretLayer2::onBack)),
        &SecretLayer2_onBack,
        "SecretLayer2::onBack",
        tulip::hook::TulipConvention::Thiscall
    );
}