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
public:
    JoinGame() : port(0) {}
    JoinGame(const std::string &host, unsigned short port)
        : host(host), port(port) {}
    
    std::string getHost() const { return host; }
    unsigned short getPort() const { return port; }
    
    virtual EventType::type_t getType() const
        { return EventType::JOIN_GAME; }
};

}  // namespace Event
}  // namespace Project

#endif
