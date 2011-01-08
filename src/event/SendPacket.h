#ifndef PROJECT_EVENT__SEND_PACKET_H
#define PROJECT_EVENT__SEND_PACKET_H

#include "EventBase.h"

namespace Project {
namespace Network {
class Packet;  // forward declaration
}
}

namespace Project {
namespace Event {

class SendPacket : public EventBase {
private:
    Network::Packet *packet;
public:
    SendPacket() : packet(0) {}
    SendPacket(Network::Packet *packet) : packet(packet) {}
    
    Network::Packet *getPacket() { return packet; }
    
    virtual EventType::type_t getType() const
        { return EventType::SEND_PACKET; }
};

}  // namespace Event
}  // namespace Project

#endif
