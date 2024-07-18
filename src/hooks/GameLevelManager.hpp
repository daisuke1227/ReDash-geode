#include <Geode/Geode.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include "../ui/RDDailyNode.hpp"
#include "../ui/RDButton.hpp"
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

        if (tag == "leaderboard_global") {
            if (response != "-1") {
                if (Variables::GlobalRank != -1) {
                    std::string responseStd = response;
                    std::string name = GJAccountManager::get()->m_username;
                    std::transform(responseStd.begin(), responseStd.end(), responseStd.begin(), [](unsigned char c) { return std::tolower(c); });
                    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
                    auto pos = responseStd.find(fmt::format("1:{}", name));
                    auto pos2 = responseStd.find("|", pos);
                    if (pos < responseStd.size()) {
                        if (pos2 >= responseStd.size()) {
                            pos2 = responseStd.size() - 1;
                        }
                        auto dict = GameLevelManager::responseToDict(responseStd.substr(pos, pos2 - pos), false);
                        Variables::GlobalRank = as<CCString*>(dict->objectForKey("6"))->intValue();
                        Variables::OldStarsCount = GameStatsManager::sharedState()->getStat("6");

                        if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
                            if (auto button = typeinfo_cast<RDButton*>(layer->getChildByID("redash-menu"_spr)->getChildByID("main-menu"_spr)->getChildByID("leaderboards-button"))) {
                                button->updateLeaderboardLabel();
                            }
                        }
                    } else {
                        log::error("Failed to find player in leaderboard response");
                        log::error("response: {}", responseStd);
                        log::error("name: {}", name);
                        log::error("pos: {}; pos2: {}", pos, pos2);
                    }
                }
            } else {
                if (Variables::GlobalRank != -1) {
                    if (auto layer = getChildOfType<MenuLayer>(CCDirector::sharedDirector()->getRunningScene(), 0)) {
                        if (auto button = typeinfo_cast<RDButton*>(layer->getChildByID("redash-menu"_spr)->getChildByID("main-menu"_spr)->getChildByID("leaderboards-button"))) {
                            button->getLeaderboardRankFailed();
                        }
                    }
                }
            }
        }
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
            if (tag == "-1_0") {
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
            } else if (tag == "-2_0") {
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
    }

    void onGetGJDailyLevelStateCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onGetGJDailyLevelStateCompleted(response, tag);

        if (response != "-1") {
            auto responseStd = std::string(response.c_str());
            auto pos = responseStd.find('|') + 1;
            if (pos >= responseStd.size()) return;
            auto timeLeft = std::stoi(responseStd.substr());

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
                            if (this->m_dailyIDUnk < this->m_dailyID && (dailyNode->m_currentLevel->m_normalPercent < 100 || GameStatsManager::sharedState()->hasCompletedDailyLevel(this->m_dailyIDUnk))) {
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
                            if (this->m_weeklyIDUnk < this->m_weeklyID  && (weeklyNode->m_currentLevel->m_normalPercent < 100 || GameStatsManager::sharedState()->hasCompletedDailyLevel(this->m_weeklyIDUnk))) {
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