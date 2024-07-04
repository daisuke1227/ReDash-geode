#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDStatsNode : public CCNode {
public:
    static RDStatsNode* create(std::string sprite, std::string string);
    bool init(std::string sprite, std::string string);
};