#ifndef PROJECT_EVENT__SETUP_CHAT_H
#define PROJECT_EVENT__SETUP_CHAT_H

#include <string>

#include "EventBase.h"

namespace Project {
namespace Event {

class SetupChat : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & client;
        ar & name;
        ar & message;
    }
private:
    int client;
    std::string name;
    std::string message;
    bool justSpoken;
public:
    SetupChat() : client(-1), justSpoken(false) {}
    SetupChat(int client, const std::string &name, const std::string &message,
        bool justSpoken = false)
        : client(client), name(name), message(message),
        justSpoken(justSpoken) {}
    
    int getClient() const { return client; }
    const std::string &getName() const { return name; }
    const std::string &getMessage() const { return message; }
    bool getJustSpoken() const { return justSpoken; }
    
    virtual EventType::type_t getType() const
        { return EventType::SETUP_CHAT; }
};

}  // namespace Event
}  // namespace Project

#endif
