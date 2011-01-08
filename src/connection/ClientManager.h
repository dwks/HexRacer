#ifndef PROJECT_CONNECTION__CLIENT_MANAGER_H
#define PROJECT_CONNECTION__CLIENT_MANAGER_H

#include <vector>
#include "Socket.h"

namespace Project {
namespace Connection {

/** Maintains a list of client sockets. Messages can be received from clients
    and broadcast to all clients at once.
*/
class ClientManager {
private:
    typedef std::vector<Socket *> socket_list_t;
    socket_list_t socket_list;
public:
    ~ClientManager();
    
    /** Registers the given socket in the list. This class takes ownership of
        @a socket and will automatically free the memory for it when the
        ClientManager is destructed.
    */
    void addSocket(Socket *socket);
    
    /** Broadcasts @a message to all registered sockets.
    */
    bool broadcast(const std::string &message);
    
    /** Checks for incoming packets on any sockets. If any packet is available,
        the parameters are set to indicate the data and true is returned.
    */
    bool tryReceive(Socket *&socket, std::string &message);
};

}  // namespace Connection
}  // namespace Project

#endif
