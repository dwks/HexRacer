#ifndef PROJECT_NETWORK__STRING_SERIALIZER_H
#define PROJECT_NETWORK__STRING_SERIALIZER_H

#include "connection/Socket.h"

namespace Project {
namespace Network {

/** Sends and receives strings of known length over a socket.
    
    The packet size is automatically sent first. When reading, the size is
    read and then the string is reconstructed. In case the size makes it across
    the socket but not the packet, this class still recovers the string in a
    non-blocking way by remembering the size.
    
    The socket passed to this class must be valid and non-NULL.
*/
class StringSerializer {
private:
    Connection::Socket *socket;
    int nextReadSize;
    static const int SIZE_BYTES = 8;
public:
    StringSerializer(Connection::Socket *socket);
    
    /** Returns true if there is another string to be read from the socket.
        
        @param data Set to the data read from the socket.
    */
    bool readNextString(std::string &data);
    
    /** Sends a string over the socket.
    */
    void sendString(const std::string &data);
};

}  // namespace Network
}  // namespace Project

#endif
