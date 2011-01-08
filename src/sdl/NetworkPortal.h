#ifndef PROJECT_SDL__NETWORK_PORTAL_H
#define PROJECT_SDL__NETWORK_PORTAL_H

#include "connection/Socket.h"
#include "network/StringSerializer.h"

namespace Project {
namespace SDL {

class NetworkPortal {
private:
    Connection::Socket *socket;
    Network::StringSerializer *stringSerializer;
public:
    NetworkPortal();
    ~NetworkPortal();
    
    /** Connects to the server hosted on @a hostname and listening on @a port.
    */
    void connectTo(const char *hostname, unsigned short port);
    
    /** Disconnects from the server.
        
        It is safe to call this function even if connectTo() has not been
        called, or if connectTo() failed.
    */
    void disconnectFrom();
    
    /** It is safe to call this function even if connectTo() has not been
        called, or if connectTo() failed.
    */
    void checkNetwork();
};

}  // namespace SDL
}  // namespace Project

#endif
