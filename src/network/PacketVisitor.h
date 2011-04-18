#ifndef PROJECT_NETWORK__PACKET_VISITOR_H
#define PROJECT_NETWORK__PACKET_VISITOR_H

namespace Project {
namespace Network {

// forward declarations
class HandshakePacket;
class EventPacket;
class PingPacket;

/** Implementation of the Visitor design pattern for Packets.
*/
class PacketVisitor {
public:
    virtual ~PacketVisitor() {}
    
    virtual void visit(HandshakePacket &packet) = 0;
    virtual void visit(EventPacket &packet) = 0;
    virtual void visit(PingPacket &packet) = 0;
};

}  // namespace Network
}  // namespace Project

#endif
