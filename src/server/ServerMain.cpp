#include <unistd.h>

#include "ServerMain.h"

#include "connection/Socket.h"
#include "connection/ServerManager.h"

namespace Project {
namespace Server {

void ServerMain::run() {
    Connection::ServerManager manager;
    
    manager.addServer(1820);
    
    for(;;) {
        for(;;) {
            Connection::Socket *socket = manager.checkForConnections();
            if(!socket) break;
            
            delete socket;
        }
        
        usleep(10000);
    }
}

}  // namespace Server
}  // namespace Project
