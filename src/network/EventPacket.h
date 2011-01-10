#ifndef PROJECT_NETWORK__EVENT_PACKET_H
#define PROJECT_NETWORK__EVENT_PACKET_H

#include "Packet.h"
#include "event/EventBase.h"

namespace Project {
namespace Network {

/** Takes any serializable Event and wraps it in a packet.
*/
class EventPacket : public Packet {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<Packet>(*this);
        ar & event;
    }
private:
    Event::EventBase *event;
public:
    EventPacket() : event(0) {}
    EventPacket(Event::EventBase *event) : event(event) {}
    
    Event::EventBase *getEvent() { return event; }
    
    virtual void accept(PacketVisitor &visitor) { visitor.visit(*this); }
};

}  // namespace Network
}  // namespace Project

#endif
