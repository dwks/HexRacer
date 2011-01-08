#include <unistd.h>

#include "ServerMain.h"

#include "connection/Socket.h"
#include "connection/ServerManager.h"
#include "connection/ClientManager.h"

#include "network/PacketSerializer.h"
#include "network/HandshakePacket.h"

namespace Project {
namespace Server {

void ServerMain::run() {
    Connection::ServerManager server;
    Connection::ClientManager clients;
    
    server.addServer(1820);
    
    for(;;) {
        for(;;) {
            Connection::Socket *socket = server.checkForConnections();
            if(!socket) break;
            
            //clients.addSocket(socket);
            
            Network::PacketSerializer serializer;
            Network::Packet *packet = new Network::HandshakePacket(-1);
            socket->send(serializer.packetToString(packet));
            
            delete socket;  // disconnect
        }
        
        usleep(10000);
    }
}

}  // namespace Server
}  // namespace Project
