#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDDailyNode : public CCNode {
protected:
    GJGameLevel* m_currentLevel;
    bool m_isWeekly;

    void onView(CCObject* sender);
public:
    static RDDailyNode* create(bool isWeekly, CCPoint position, CCSize size);
    bool init(bool isWeekly, CCPoint position, CCSize size);

};