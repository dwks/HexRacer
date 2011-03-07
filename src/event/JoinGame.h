#ifndef PROJECT_EVENT__JOIN_GAME_H
#define PROJECT_EVENT__JOIN_GAME_H

#include <string>

#include "EventBase.h"

namespace Project {
namespace Event {

class JoinGame : public EventBase {
private:
    std::string host;
    unsigned short port;
    bool success;
public:
    JoinGame() : port(0), success(false) {}
    JoinGame(const std::string &host, unsigned short port)
        : host(host), port(port), success(false) {}
    
    std::string getHost() const { return host; }
    unsigned short getPort() const { return port; }
    bool getSuccess() const { return success; }
    
    void setSuccessful() { success = true; }
    
    virtual EventType::type_t getType() const
        { return EventType::JOIN_GAME; }
};

}  // namespace Event
}  // namespace Project

#endif
