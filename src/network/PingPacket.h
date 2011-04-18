#ifndef PROJECT_NETWORK__PING_PACKET_H
#define PROJECT_NETWORK__PING_PACKET_H

#include "Packet.h"

namespace Project {
namespace Network {

class PingPacket : public Packet {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<Packet>(*this);
        ar & milliseconds;
    }
private:
    unsigned long milliseconds;
public:
    PingPacket() {}
    
    void setMilliseconds(unsigned long ms) { milliseconds = ms; }
    unsigned long getMilliseconds() const { return milliseconds; }
    
    virtual void accept(PacketVisitor &visitor) { visitor.visit(*this); }
};

}  // namespace Network
}  // namespace Project

#endif
