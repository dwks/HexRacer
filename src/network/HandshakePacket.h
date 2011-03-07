#ifndef PROJECT_NETWORK__HANDSHAKE_PACKET_H
#define PROJECT_NETWORK__HANDSHAKE_PACKET_H

#include <string>

#include "Packet.h"

namespace Project {
namespace Network {

/** Packet sent from server to client to acknowledge connection.
*/
class HandshakePacket : public Packet {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<Packet>(*this);
        ar & clientID;
        ar & map;
        ar & milliseconds;
    }
private:
    int clientID;
    std::string map;
    unsigned long milliseconds;
public:
    HandshakePacket() {}
    HandshakePacket(int clientID, const std::string &map,
        unsigned long milliseconds)
        : clientID(clientID), map(map), milliseconds(milliseconds) {}
    
    int getClientID() const { return clientID; }
    const std::string &getMap() const { return map; }
    unsigned long getMilliseconds() const { return milliseconds; }
    
    virtual void accept(PacketVisitor &visitor) { visitor.visit(*this); }
};

}  // namespace Network
}  // namespace Project

#endif
