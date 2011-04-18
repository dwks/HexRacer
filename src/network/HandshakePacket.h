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
        ar & erasingEnabled & overwriteEnabled & bonusEnabled;
        ar & milliseconds;
    }
private:
    int clientID;
    std::string map;
    bool erasingEnabled, overwriteEnabled, bonusEnabled;
    unsigned long milliseconds;
public:
    HandshakePacket() {}
    HandshakePacket(int clientID, const std::string &map,
        bool erasingEnabled, bool overwriteEnabled, bool bonusEnabled,
        unsigned long milliseconds)
        : clientID(clientID), map(map), erasingEnabled(erasingEnabled),
        overwriteEnabled(overwriteEnabled), bonusEnabled(bonusEnabled),
        milliseconds(milliseconds) {}
    
    bool isErasingEnabled() const { return erasingEnabled; }
    bool isOverwriteEnabled() const { return overwriteEnabled; }
    bool isBonusEnabled() const { return bonusEnabled; }
    
    int getClientID() const { return clientID; }
    const std::string &getMap() const { return map; }
    unsigned long getMilliseconds() const { return milliseconds; }
    
    virtual void accept(PacketVisitor &visitor) { visitor.visit(*this); }
};

}  // namespace Network
}  // namespace Project

#endif
