#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDMainButton : public CCMenuItemSpriteExtra {
public:
    static RDMainButton* create(CCPoint position, CCSize size, std::string id, float scale);
    bool init(CCPoint position, CCSize size, std::string id, float scale);

    void onPlay(CCObject* sender);
};