#include <Geode/Geode.hpp>
#include "../Variables.hpp"
using namespace geode::prelude;

class RDDailyNode : public CCNode {
protected:
    GJGameLevel* m_currentLevel;
    bool m_isWeekly;
    CCLabelBMFont* m_timeLabel;

public:
    void onView(CCObject* sender);
    void updateTimeLabel(float time);

    static RDDailyNode* create(bool isWeekly, CCPoint position, CCSize size, std::string id);
    bool init(bool isWeekly, CCPoint position, CCSize size, std::string id);
};