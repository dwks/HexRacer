#ifndef PROJECT_EVENT__GENERAL_WORLD_SETUP_EVENT_H
#define PROJECT_EVENT__GENERAL_WORLD_SETUP_EVENT_H

#include <string>

#include "EventBase.h"

namespace Project {
namespace Event {

class GeneralWorldSetupEvent : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & client;
        ar & data;
    }
private:
    int client;
    std::string data;
public:
    GeneralWorldSetupEvent() {}
    GeneralWorldSetupEvent(int client, const std::string &data)
        : client(client), data(data) {}
    
    int getClient() const { return client; }
    std::string getData() const { return data; }
    
    virtual EventType::type_t getType() const
        { return EventType::GENERAL_WORLD_SETUP; }
};

}  // namespace Event
}  // namespace Project

#endif
