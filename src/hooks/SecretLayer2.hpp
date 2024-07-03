#include <Geode/Geode.hpp>
#include <Geode/modify/SecretLayer2.hpp>
using namespace geode::prelude;

// my fucking fault
// it doesnt even work lmao
void SecretLayer2_onBack(SecretLayer2* self, cocos2d::CCObject* sender) {
    log::info("onBack");
    CCScene* sc = MenuLayer::scene(false);
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