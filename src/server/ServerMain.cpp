#include <unistd.h>

#include "ServerMain.h"
#include "ClientManager.h"

#include "connection/Socket.h"
#include "connection/ServerManager.h"

#include "network/StringSerializer.h"
#include "network/PacketSerializer.h"
#include "network/HandshakePacket.h"

#include "log/Logger.h"

namespace Project {
namespace Server {

void ServerMain::run() {
    Connection::ServerManager server;
    ClientManager clients;
    
    server.addServer(1820);
    
    for(;;) {
        for(;;) {
            Connection::Socket *socket = server.checkForConnections();
            if(!socket) break;
            
            Network::PacketSerializer packetSerializer;
            Network::Packet *packet = new Network::HandshakePacket(clientCount);
            clientCount ++;
            
            Network::StringSerializer stringSerializer(socket);
            stringSerializer.sendString(
                packetSerializer.packetToString(packet));
            
            clients.addClient(socket);
            //delete socket;  // disconnect
        }
        
        {
            int whichSocket;
            Network::Packet *packet;
            while((packet = clients.nextPacket(&whichSocket))) {
                LOG(NETWORK, "Packet received from "
                    << whichSocket << ": \"" << packet << "\"");
            }
        }
        
        usleep(10000);
    }
}

}  // namespace Server
}  // namespace Project
