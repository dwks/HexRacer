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
    
    typedef std::vector<unsigned long> timeout_t;
    timeout_t timeout;
    
    typedef std::vector<int> gameid_t;
    gameid_t gameid;
    
    unsigned long allowableTimeout;
public:
    ClientManager();
    ~ClientManager();
    
    void addClient(Connection::Socket *socket, int id);
    int getGameID(int which);
    void forceDisconnect(int which);
    
    void sendPacket(Network::Packet *packet);
    void sendPacketExcept(Network::Packet *packet, int exception);
    void sendPacketOnly(Network::Packet *packet, int which);
    
    void setPingTimeout(int client, unsigned long ms);
    void setAllowableTimeout(unsigned long allowableTimeout);
    
    int nextDisconnectedClient(int &gid);
    Network::Packet *nextPacket(int *whichSocket = NULL);
    
    bool socketExists(int which) const;
    int getSocketCount() const;
};

}  // namespace Server
}  // namespace Project

#endif
