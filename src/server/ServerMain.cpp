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
#include "event/ObserverList.h"

#include "object/PlayerList.h"

#include "log/Logger.h"
#include "misc/Sleeper.h"

namespace Project {
namespace Server {

void ServerMain::ServerVisitor::visit(Network::HandshakePacket &packet) {
    LOG2(NETWORK, ERROR,
        "Server received HandshakePacket, this should never happen");
}

void ServerMain::ServerVisitor::visit(Network::EventPacket &packet) {
    // bootstrap into event subsystem
    
    //EMIT_EVENT(packet.getEvent());
    
    // don't free this Event
    Event::ObserverList::getInstance().notifyObservers(
        packet.getEvent(), false);
}

void ServerMain::ServerObserver::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::PLAYER_MOVEMENT: {
        Event::PlayerMovement *movement
            = dynamic_cast<Event::PlayerMovement *>(event);
        main->getPlayerList().getPlayer(main->getWhichSocket())
            ->addPosition(movement->getMovement());
        break;
    }
    default:
        LOG2(NETWORK, WARNING,
            "Don't know how to handle events of type " << event->getType());
        break;
    }
}

bool ServerMain::ServerObserver::interestedIn(Event::EventType::type_t type) {
    return true;
}

void ServerMain::run() {
    Connection::ServerManager server;
    ClientManager clients;
    
    server.addServer(1820);
    
    ADD_OBSERVER(new ServerObserver(this));
    
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
            playerList.addPlayer(new Object::Player(clientCount));
            
            clientCount ++;
        }
        
        {
            Network::Packet *packet;
            while((packet = clients.nextPacket(&whichSocket))) {
                LOG(NETWORK, "Packet received from "
                    << whichSocket << ": " << packet);
                
                packet->accept(visitor);
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
        
        Misc::Sleeper::sleep(10 * 1000);
    }
}

}  // namespace Server
}  // namespace Project
