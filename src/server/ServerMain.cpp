#include <unistd.h>

#include "ServerMain.h"
#include "ClientManager.h"

#include "connection/Socket.h"
#include "connection/ServerManager.h"

#include "network/StringSerializer.h"
#include "network/PacketSerializer.h"
#include "network/HandshakePacket.h"
#include "network/EventPacket.h"

#include "event/PlayerMovement.h"
#include "event/UpdatePlayerList.h"

#include "object/PlayerList.h"

#include "log/Logger.h"

namespace Project {
namespace Server {

void ServerMain::run() {
    Connection::ServerManager server;
    ClientManager clients;
    Object::PlayerList playerList;
    
    server.addServer(1820);
    
    int loops = 0;
    for(;;) {
        for(;;) {
            Connection::Socket *socket = server.checkForConnections();
            if(!socket) break;
            
            Network::PacketSerializer packetSerializer;
            Network::Packet *packet = new Network::HandshakePacket(clientCount);
            
            Network::StringSerializer stringSerializer(socket);
            stringSerializer.sendString(
                packetSerializer.packetToString(packet));
            
            clients.addClient(socket);
            playerList.addPlayer(
                new Object::Player(clientCount, Math::Point()));
            //delete socket;  // disconnect
            
            clientCount ++;
        }
        
        {
            int whichSocket;
            Network::Packet *packet;
            while((packet = clients.nextPacket(&whichSocket))) {
                LOG(NETWORK, "Packet received from "
                    << whichSocket << ": \"" << packet << "\"");
                
                Event::EventBase *event
                    = dynamic_cast<Network::EventPacket *>(packet)->getEvent();
                Event::PlayerMovement *move
                    = dynamic_cast<Event::PlayerMovement *>(event);
                
                playerList.getPlayer(whichSocket)
                    ->addPosition(move->getMovement());
            }
        }
        
        if(++loops == 10) {
            loops = 0;
            
            Event::UpdatePlayerList *update
                = new Event::UpdatePlayerList(&playerList);
            Network::Packet *packet = new Network::EventPacket(update);
            clients.sendPacket(packet);
            delete packet;
        }
        
        usleep(10000);
    }
}

}  // namespace Server
}  // namespace Project
