#ifndef PROJECT_EVENT__PACKET_RECEIVED_H
#define PROJECT_EVENT__PACKET_RECEIVED_H

#include "EventBase.h"

namespace Project {
namespace Network {
class Packet;  // forward declaration
}
}

namespace Project {
namespace Event {

class PacketReceived : public EventBase {
private:
    Network::Packet *packet;
public:
    PacketReceived() : packet(0) {}
    PacketReceived(Network::Packet *packet) : packet(packet) {}
    
    Network::Packet *getPacket() { return packet; }
    
    virtual EventType::type_t getType() const
        { return EventType::PACKET_RECEIVED; }
};

}  // namespace Event
}  // namespace Project

#endif
