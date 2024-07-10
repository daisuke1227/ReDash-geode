#include <Geode/Geode.hpp>
using namespace geode::prelude;

class RDMainButton : public CCMenuItemSpriteExtra {
public:
    static RDMainButton* create(CCPoint position, CCSize size, std::string id);
    bool init(CCPoint position, CCSize size, std::string id);

    void onPlay(CCObject* sender);
};