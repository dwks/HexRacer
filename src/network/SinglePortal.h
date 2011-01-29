#ifndef PROJECT_NETWORK__SINGLE_PORTAL_H
#define PROJECT_NETWORK__SINGLE_PORTAL_H

#include "Packet.h"
#include "connection/Socket.h"
#include "StringSerializer.h"
#include "PacketSerializer.h"

namespace Project {
namespace Network {

/** Wrapper around a Socket that deals at the level of Packets.
    
    Packets are turned into std::strings with a PacketSerializer, and then the
    std::strings are sent over the network with a StringSerializer. Receiving
    packets works the same way, in reverse.
    
    This class does not own its Socket and will not free it.
*/
class SinglePortal {
private:
    Connection::Socket *socket;
    StringSerializer *stringSerializer;
    PacketSerializer packetSerializer;
public:
    SinglePortal(Connection::Socket *socket);
    ~SinglePortal();
    
    /** Sends the given packet along this socket.
    */
    void sendPacket(Packet *packet);
    
    /** Returns true if the socket behind this portal is still open.
    */
    bool isOpen();
    
    /** Returns the next packet waiting to be read from this socket, or NULL
        if there is no packet currently available.
    */
    Packet *nextPacket();
};

}  // namespace Network
}  // namespace Project

#endif
