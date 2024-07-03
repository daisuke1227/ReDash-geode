#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDDailyButton : public CCNode {
protected:
    GJGameLevel* m_currentLevel;
    bool m_isWeekly;

    void onView(CCObject* sender);
public:
    static RDDailyButton* create(bool isWeekly, CCPoint position, CCSize size);
    bool init(bool isWeekly, CCPoint position, CCSize size);

};