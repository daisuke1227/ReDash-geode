#include <Geode/Geode.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include "../ui/RDDailyNode.hpp"
using namespace geode::prelude;

class $modify(MyGLM, GameLevelManager) {
    void updateTimers() {
        TimelyLeft::Daily--;
        TimelyLeft::Weekly--;

        if (TimelyLeft::Weekly < 1) {
		    GameLevelManager::getGJDailyLevelState(GJTimedLevelType::Weekly);
		}
		if (TimelyLeft::Daily < 1) {
		    GameLevelManager::getGJDailyLevelState(GJTimedLevelType::Daily);
		}
    }

    void onGetGJDailyLevelStateCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onGetGJDailyLevelStateCompleted(response, tag);

        auto timeLeft = std::stoi(response.substr(response.find('|') + 1));

        if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
            if (tag == "daily_state") {
                TimelyLeft::Daily = timeLeft;
                if (auto dailyNode = typeinfo_cast<RDDailyNode*>(layer->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->getChildByID("daily-node"))) {
                    dailyNode->updateTimeLabel(1.f);
                    dailyNode->schedule(schedule_selector(RDDailyNode::updateTimeLabel), 1.f);
                }
            } else if (tag == "weekly_state") {
                TimelyLeft::Weekly = timeLeft;
                if (auto weeklyNode = typeinfo_cast<RDDailyNode*>(layer->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->getChildByID("weekly-node"))) {
                    weeklyNode->updateTimeLabel(1.f);
                    weeklyNode->schedule(schedule_selector(RDDailyNode::updateTimeLabel), 1.f);
                }
            }
        }

        CCScheduler::get()->scheduleSelector(schedule_selector(MyGLM::updateTimers), this, 1.f, false);
    }
};