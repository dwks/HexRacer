#ifndef PROJECT_EVENT__SETUP_CLIENT_SETTINGS_H
#define PROJECT_EVENT__SETUP_CLIENT_SETTINGS_H

#include "EventBase.h"
#include "world/WorldSetup.h"

namespace Project {
namespace Event {

class SetupClientSettings : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & clientSettings;
    }
private:
    World::WorldSetup::ClientSettings clientSettings;
public:
    SetupClientSettings() {}
    SetupClientSettings(World::WorldSetup::ClientSettings clientSettings)
        : clientSettings(clientSettings) {}
    
    virtual World::WorldSetup::ClientSettings getClientSettings() const
        { return clientSettings; }
    
    virtual EventType::type_t getType() const
        { return EventType::SETUP_CLIENT_SETTINGS; }
};

}  // namespace Event
}  // namespace Project

#endif
