#ifndef PROJECT_SERVER__CLIENT_MANAGER_H
#define PROJECT_SERVER__CLIENT_MANAGER_H

#include <vector>

#include "connection/Socket.h"
#include "network/SinglePortal.h"

namespace Project {
namespace Server {

/** This class provides essentially the same interface as SinglePortal, except
    that this operates on all the client sockets at once.
*/
class ClientManager {
private:
    typedef std::vector<Network::SinglePortal *> portal_list_t;
    portal_list_t portal_list;
public:
    ~ClientManager();
    
    void addClient(Connection::Socket *socket);
    
    void sendPacket(Network::Packet *packet);
    
    Network::Packet *nextPacket(int *whichSocket = NULL);
};

}  // namespace Server
}  // namespace Project

#endif
