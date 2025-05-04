#include <Geode/Geode.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include <Macros.hpp>

#include "../ui/timely/RDTimelyNode.hpp"
#include "../ui/timely/RDDailyNode.hpp"
#include "../ui/timely/RDWeeklyNode.hpp"
#include "../ui/timely/RDEventNode.hpp"
#include "../ui/RDButton.hpp"
using namespace geode::prelude;

class $modify(MyGLM, GameLevelManager) {
public:
    // Use struct Fields to add custom members (per Geode modify guidelines) ([docs.geode-sdk.org](https://docs.geode-sdk.org/tutorials/fields/?utm_source=chatgpt.com))
    struct Fields {
        int m_dailyIDUnk   = 0;
        int m_weeklyIDUnk  = 0;
        int m_eventIDUnk   = 0;
    };

    void updateDailyTimer() {
        Variables::DailyLeft--;
        if (Variables::DailyLeft < 0) {
            CCScheduler::get()->unscheduleSelector(schedule_selector(MyGLM::updateDailyTimer), this);
            GameLevelManager::getGJDailyLevelState(GJTimedLevelType::Daily);
        }
    }

    void updateWeeklyTimer() {
        Variables::WeeklyLeft--;
        if (Variables::WeeklyLeft < 1) {
            CCScheduler::get()->unscheduleSelector(schedule_selector(MyGLM::updateWeeklyTimer), this);
            GameLevelManager::getGJDailyLevelState(GJTimedLevelType::Weekly);
        }
    }

    void updateEventTimer() {
        Variables::EventLeft--;
        if (auto scene = CCDirector::sharedDirector()->getRunningScene()) {
            if (auto layer = scene->getChildByType<MenuLayer>(0)) {
                if (Variables::EventLeft < 1 && Mod::get()->getSettingValue<bool>("events-check")) {
                    CCScheduler::get()->unscheduleSelector(schedule_selector(MyGLM::updateEventTimer), this);
                    GameLevelManager::getGJDailyLevelState(GJTimedLevelType::Event);
                }
            }
        }
    }

    void onGetGJChallengesCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onGetGJChallengesCompleted(response, tag);
        Variables::didGetGJChallenge = true;
        for (auto page : Variables::challengesPages) {
            if (page && page->retainCount() > 0) page->release();
        }
        Variables::challengesPages.clear();
    }

    void getGJChallenges() {
        GameLevelManager::getGJChallenges();
        Variables::didGetGJChallenge = false;
    }

    void onGetLeaderboardScoresCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onGetLeaderboardScoresCompleted(response, tag);
        if (tag == "leaderboard_global") {
            if (response != "-1" && Variables::GlobalRank != -1) {
                std::string resp = response;
                std::string name = GJAccountManager::get()->m_username;
                std::transform(resp.begin(), resp.end(), resp.begin(), ::tolower);
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                auto pos = resp.find(fmt::format("1:{}", name));
                auto pos2 = resp.find("|", pos);
                if (pos < resp.size()) {
                    if (pos2 >= resp.size()) pos2 = resp.size() - 1;
                    auto dict = GameLevelManager::responseToDict(resp.substr(pos, pos2 - pos), false);
                    Variables::GlobalRank = as<CCString*>(dict->objectForKey("6"))->intValue();
                    Variables::OldStarsCount = GameStatsManager::sharedState()->getStat("6");
                    if (auto layer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<MenuLayer>(0)) {
                        if (auto btn = typeinfo_cast<RDButton*>(layer
                                ->getChildByID("redash-menu"_spr)
                                ->getChildByID("main-menu"_spr)
                                ->getChildByID("leaderboards-button"))) {
                            btn->updateLeaderboardLabel();
                        }
                    }
                }
            } else if (Variables::GlobalRank != -1) {
                if (auto layer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<MenuLayer>(0)) {
                    if (auto btn = typeinfo_cast<RDButton*>(layer
                            ->getChildByID("redash-menu"_spr)
                            ->getChildByID("main-menu"_spr)
                            ->getChildByID("leaderboards-button"))) {
                        btn->getLeaderboardRankFailed();
                    }
                }
            }
        }
    }

    void processOnDownloadLevelCompleted(gd::string response, gd::string tag, bool p2) {
        GameLevelManager::processOnDownloadLevelCompleted(response, tag, p2);
        if (response != "-1") {
            if (tag == "-1_0") {
                m_fields->m_dailyIDUnk = this->m_dailyID;
            } else if (tag == "-2_0") {
                m_fields->m_weeklyIDUnk = this->m_weeklyID;
            }
        }
        if (auto layer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<MenuLayer>(0)) {
            if (tag == "-1_0") {
                RD_HANDLE_LEVEL("daily-node", m_fields->m_dailyIDUnk);
            } else if (tag == "-2_0") {
                RD_HANDLE_LEVEL("weekly-node", m_fields->m_weeklyIDUnk);
            } else if (tag == "-3_0") {
                RD_HANDLE_LEVEL("event-node", m_fields->m_eventIDUnk);
            }
        }
    }

    void onGetGJDailyLevelStateCompleted(gd::string response, gd::string tag) {
        GameLevelManager::onGetGJDailyLevelStateCompleted(response, tag);
        if (response == "-1") return;
        auto s = std::string(response.c_str());
        auto pos = s.find('|') + 1;
        if (pos >= s.size()) return;
        int timeLeft = std::stoi(s.substr(pos));

        if (tag == "daily_state") {
            if (m_fields->m_dailyIDUnk == 0) this->downloadLevel(-1, false);
            Variables::DailyLeft = timeLeft;
            CCScheduler::get()->scheduleSelector(schedule_selector(MyGLM::updateDailyTimer), this, 1.f, false);
            RD_HANDLE_GET_DAILY(RDDailyNode, "daily-node", this->m_dailyID, m_fields->m_dailyIDUnk);

        } else if (tag == "weekly_state") {
            if (m_fields->m_weeklyIDUnk == 0) this->downloadLevel(-2, false);
            Variables::WeeklyLeft = timeLeft;
            CCScheduler::get()->scheduleSelector(schedule_selector(MyGLM::updateWeeklyTimer), this, 1.f, false);
            RD_HANDLE_GET_DAILY(RDWeeklyNode, "weekly-node", this->m_weeklyID, m_fields->m_weeklyIDUnk);

        } else if (tag == "event_state") {
            if (m_fields->m_eventIDUnk == 0) this->downloadLevel(-3, false);
            Variables::EventLeft = timeLeft;
            CCScheduler::get()->scheduleSelector(schedule_selector(MyGLM::updateEventTimer), this, 1.f, false);
            if (auto layer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<MenuLayer>(0)) {
                if (auto node = typeinfo_cast<RDEventNode*>(layer
                        ->getChildByID("redash-menu"_spr)
                        ->getChildByID("dailies-menu"_spr)
                        ->getChildByID("event-node"))) {
                    if (node->m_skipButton) {
                        if (m_fields->m_eventIDUnk < this->m_eventID
                            && (node->m_currentLevel->m_normalPercent < 100
                            || GameStatsManager::sharedState()->hasCompletedDailyLevel(m_fields->m_eventIDUnk))) {
                            node->m_skipButton->setVisible(true);
                        } else {
                            node->m_skipButton->setVisible(false);
                        }
                    }
                }
            }
        }
    }
};