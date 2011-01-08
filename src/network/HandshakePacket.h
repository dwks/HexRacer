#ifndef PROJECT_NETWORK__HANDSHAKE_PACKET_H
#define PROJECT_NETWORK__HANDSHAKE_PACKET_H

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
    }
private:
    int clientID;
public:
    HandshakePacket() {}
    HandshakePacket(int clientID) : clientID(clientID) {}
    
    int getClientID() const { return clientID; }
    
    virtual void accept(PacketVisitor &visitor) { visitor.visit(*this); }
};

}  // namespace Network
}  // namespace Project

#endif
