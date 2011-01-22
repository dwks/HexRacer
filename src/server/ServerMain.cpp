#include "boost/asio.hpp"

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

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "object/PlayerList.h"

#include "render/MeshGroup.h"
#include "render/MeshLoader.h"

#include "log/Logger.h"
#include "misc/Sleeper.h"

#include "config.h"

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
            ->applyMovement(movement->getMovement());
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
    Physics::PhysicsWorld *physicsWorld = new Physics::PhysicsWorld();
    physicsWorld->createTestScene();
    
    Connection::ServerManager server;
    ClientManager clients;
    
    server.addServer(1820);
    
    Render::MeshLoader *meshLoader = new Render::MeshLoader();
    meshLoader->loadOBJ("testTerrain", "models/testterrain.obj");
    Render::MeshGroup *test_terrain = meshLoader->getModelByName("testTerrain");
    
    Physics::PhysicsWorld::getInstance()->registerRigidBody(
        Physics::PhysicsFactory::createRigidTriMesh(
            test_terrain->getTriangles()));
    
    ADD_OBSERVER(new ServerObserver(this));
    
    int loops = 0;
    unsigned long lastTime = Misc::Sleeper::getTimeMilliseconds();
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
                new Object::Player(clientCount, INITIAL_CAR_LOCATION));
            
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
        
        {
            static unsigned long lastPhysicsTime
                = Misc::Sleeper::getTimeMilliseconds();
            unsigned long thisTime = Misc::Sleeper::getTimeMilliseconds();
            physicsWorld->stepWorld((thisTime - lastPhysicsTime) * 1000);
            lastPhysicsTime = thisTime;
        }
        
        if(++loops == 10) {
            loops = 0;
            
            for(int p = 0; p < clientCount; p ++) {
                LOG(PHYSICS, "Player " << p << " is at "
                    << getPlayerList().getPlayer(p)->getPosition());
            }
            
            Event::UpdatePlayerList *update
                = new Event::UpdatePlayerList(
                    Misc::Sleeper::getTimeMilliseconds(), &playerList);
            Network::Packet *packet = new Network::EventPacket(update);
            clients.sendPacket(packet);
            delete packet;
        }
        
        {
            unsigned long currentTime = Misc::Sleeper::getTimeMilliseconds();
            unsigned long elapsed = currentTime - lastTime;
            long tosleep = 10 - elapsed;
            if(tosleep > 0) {
                Misc::Sleeper::sleep(tosleep);
            }
            
            //while(lastTime < currentTime) lastTime += 10;
            while(lastTime < currentTime + tosleep) lastTime += 10;
        }
    }
}

}  // namespace Server
}  // namespace Project
