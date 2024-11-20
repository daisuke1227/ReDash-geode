#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../Variables.hpp"
using namespace geode::prelude;

class $modify(CreatorLayer) {
    void onChallenge(CCObject* sender) {
        if (auto layer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<MenuLayer>(0)) {
            auto popup = ChallengesPage::create();
            popup->show();
            if (!Variables::didGetGJChallenge) {
                Variables::challengesPages.push_back(popup);
                popup->retain();
            }
            return;
        }
        CreatorLayer::onChallenge(sender);
    }
};