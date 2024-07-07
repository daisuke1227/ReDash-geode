#include <Geode/Geode.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include "../ui/RDDailyNode.hpp"
using namespace geode::prelude;

class $modify(MyGLM, GameLevelManager) {
    void updateTimers() {
        Variables::DailyLeft--;
        Variables::WeeklyLeft--;

        if (Variables::WeeklyLeft < 1) {
		    GameLevelManager::getGJDailyLevelState(GJTimedLevelType::Weekly);
		}
		if (Variables::DailyLeft < 1) {
		    GameLevelManager::getGJDailyLevelState(GJTimedLevelType::Daily);
		};
    }

    void onGetLeaderboardScoresCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onGetLeaderboardScoresCompleted(response, tag);

        log::info("{}: {}", tag, response);
        GameLevelManager::responseToDict(response, false);
    }

    // /* 0x158 0x4 int */ int	m_dailyTimeLeft;
	// /* 0x15c 0x4 int */ int	m_dailyID;
    // /* 0x160 0x4 int */ int	m_dailyIDUnk;
	// /* 0x164 0x4 int */ int	m_weeklyTimeLeft;
	// /* 0x168 0x4 int */ int	m_weeklyID;
	// /* 0x16c 0x4 int */ int	m_weeklyIDUnk;
	// /* 0x170 0x4 int */ int	m_eventTimeLeft;	
	// /* 0x174 0x4 int */ int	m_eventID;
	// /* 0x178 0x4 int */ int	m_eventIDUnk;

    void processOnDownloadLevelCompleted(gd::string response, gd::string tag, bool p2) {
        GameLevelManager::processOnDownloadLevelCompleted(response, tag, p2);

        if (response != "-1") {
            if (tag == "-1_0") {
                this->m_dailyIDUnk = this->m_dailyID;
            } else if (tag == "-2_0") {
                this->m_weeklyIDUnk = this->m_weeklyID;
            }
        }

        if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
            if (auto dailyNode = typeinfo_cast<RDDailyNode*>(layer->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->getChildByID("daily-node"))) {
                if (response != "-1") {
                    if (auto level = this->getSavedDailyLevel(this->m_dailyIDUnk)) {
                        dailyNode->setupLevelMenu(level);
                    } else {
                        log::error("where did my level go??");
                    }
                } else {
                    dailyNode->downloadLevelFailed();
                }
            }
            if (auto weeklyNode = typeinfo_cast<RDDailyNode*>(layer->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->getChildByID("weekly-node"))) {
                if (response != "-1") {
                    if (auto level = this->getSavedDailyLevel(this->m_weeklyIDUnk)) {
                        weeklyNode->setupLevelMenu(level);
                    } else {
                        log::error("where did my level go??");
                    };
                } else {
                    weeklyNode->downloadLevelFailed();
                }
            }
        }
    }

    void onGetGJDailyLevelStateCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onGetGJDailyLevelStateCompleted(response, tag);

        auto responseStd = std::string(response.c_str());
        auto timeLeft = std::stoi(responseStd.substr(responseStd.find('|') + 1));

        if (response != "-1") {
            if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
                if (tag == "daily_state") {
                    if (this->m_dailyIDUnk == 0) {
                        this->downloadLevel(-1, false);
                    }

                    Variables::DailyLeft = timeLeft;

                    if (auto dailyNode = typeinfo_cast<RDDailyNode*>(layer->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->getChildByID("daily-node"))) {
                        dailyNode->updateTimeLabel(1.f);
                        dailyNode->schedule(schedule_selector(RDDailyNode::updateTimeLabel), 1.f);

                        if (dailyNode->m_skipButton) {
                            if (this->m_dailyIDUnk < this->m_dailyID) {
                                dailyNode->m_skipButton->setVisible(true);
                            } else {
                                dailyNode->m_skipButton->setVisible(false);
                            }
                        }
                    }
                } else if (tag == "weekly_state") {
                    if (this->m_weeklyIDUnk == 0) {
                        this->downloadLevel(-2, false);
                    }
                    
                    Variables::WeeklyLeft = timeLeft;

                    if (auto weeklyNode = typeinfo_cast<RDDailyNode*>(layer->getChildByID("redash-menu"_spr)->getChildByID("dailies-menu"_spr)->getChildByID("weekly-node"))) {
                        weeklyNode->updateTimeLabel(1.f);
                        weeklyNode->schedule(schedule_selector(RDDailyNode::updateTimeLabel), 1.f);

                        if (weeklyNode->m_skipButton) {
                            if (this->m_weeklyIDUnk < this->m_weeklyID) {
                                weeklyNode->m_skipButton->setVisible(true);
                            } else {
                                weeklyNode->m_skipButton->setVisible(false);
                            }
                        }
                    }
                }
            }

            CCScheduler::get()->scheduleSelector(schedule_selector(MyGLM::updateTimers), this, 1.f, false);
        }
    }
};