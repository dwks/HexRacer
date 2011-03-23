#ifndef PROJECT_EVENT__SETUP_PLAYER_SETTINGS_H
#define PROJECT_EVENT__SETUP_PLAYER_SETTINGS_H

#include "EventBase.h"
#include "world/WorldSetup.h"

namespace Project {
namespace Event {

class SetupPlayerSettings : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & playerSettings;
    }
private:
    World::WorldSetup::PlayerSettings playerSettings;
public:
    SetupPlayerSettings() {}
    SetupPlayerSettings(World::WorldSetup::PlayerSettings playerSettings)
        : playerSettings(playerSettings) {}
    
    virtual World::WorldSetup::PlayerSettings getPlayerSettings() const
        { return playerSettings; }
    
    virtual EventType::type_t getType() const
        { return EventType::SETUP_PLAYER_SETTINGS; }
};

}  // namespace Event
}  // namespace Project

#endif
