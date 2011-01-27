#include "boost/asio.hpp"

#include "ServerMain.h"
#include "ClientManager.h"

#include "connection/Socket.h"
#include "connection/ServerManager.h"

#include "network/StringSerializer.h"
#include "network/PacketSerializer.h"
#include "network/HandshakePacket.h"
#include "network/EventPacket.h"

#include "event/QuitEvent.h"
#include "event/PlayerAction.h"
#include "event/UpdatePlayerList.h"
#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"

#include "event/ObserverList.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"
#include "physics/Suspension.h"

#include "render/MeshGroup.h"
#include "render/MeshLoader.h"

#include "log/Logger.h"
#include "misc/Sleeper.h"

#include "config.h"
#include "settings/SettingsManager.h"

#include <signal.h>

namespace Project {
namespace Server {

void terminationHandler(int signal) {
    LOG2(GLOBAL, PROGRESS,
        "Server terminating after receiving signal " << signal);
    EMIT_EVENT(new Event::QuitEvent());
}

void ServerMain::ServerVisitor::visit(Network::HandshakePacket &packet) {
    LOG2(NETWORK, ERROR,
        "Server received HandshakePacket, this should never happen");
}

void ServerMain::ServerVisitor::visit(Network::EventPacket &packet) {
    // bootstrap into event subsystem
    EMIT_EVENT(packet.getEvent());
    
    /*// don't free this Event
    Event::ObserverList::getInstance().notifyObservers(
        packet.getEvent(), false);*/
}

void ServerMain::ServerObserver::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::QUIT:
        main->setQuit();
        break;
    case Event::EventType::PLAYER_ACTION: {
        Event::PlayerAction *action
            = dynamic_cast<Event::PlayerAction *>(event);
        Object::Player *player = main->getPlayerList()
            .getPlayer(main->getWhichSocket());
        
        switch(action->getMovementType()) {
        case Event::PlayerAction::ACCELERATE:
            player->applyAcceleration(action->getValue());
            break;
        case Event::PlayerAction::TURN:
            player->applyTurning(action->getValue());
            break;
        case Event::PlayerAction::JUMP:
            player->doJump();
            break;
        }
        
        break;
    }
    default:
        LOG2(NETWORK, WARNING,
            "Don't know how to handle events of type " << event->getType());
        break;
    }
}

bool ServerMain::ServerObserver::interestedIn(Event::EventType::type_t type) {
    switch(type) {
    case Event::EventType::PAINT_EVENT:
    case Event::EventType::TOGGLE_PAINT:  // handled by PaintSubsystem
        return false;
    default:
        break;
    }
    
    return true;
}

ServerMain::ServerMain() : clientCount(0), visitor(this) {
#ifdef WIN32
    signal(SIGINT, terminationHandler);
    signal(SIGTERM, terminationHandler);
#else
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_handler = terminationHandler;
    action.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
#endif
}

ServerMain::~ServerMain() {
    delete networkPortal;
}

void ServerMain::run() {
    Physics::PhysicsWorld *physicsWorld = new Physics::PhysicsWorld();
    physicsWorld->createTestScene();
    
    Connection::ServerManager server;
    ClientManager clients;
    
    server.addServer(GET_SETTING("network.port", 1820));
    
    networkPortal = new ServerNetworkPortal(&clients);
    
    Render::MeshLoader *meshLoader = new Render::MeshLoader();
    meshLoader->loadOBJ("testTerrain", GET_SETTING("map", "models/testterrain.obj"));
    Render::MeshGroup *test_terrain = meshLoader->getModelByName("testTerrain");
    
    Physics::PhysicsWorld::getInstance()->registerRigidBody(
        Physics::PhysicsFactory::createRigidTriMesh(
            test_terrain->getTriangles()));
    
    ADD_OBSERVER(new ServerObserver(this));
    
    Physics::Suspension suspension;
    
    worldManager = new Object::WorldManager();
    paintSubsystem = new Paint::PaintSubsystem(worldManager, 20);
    
    int loops = 0;
    unsigned long lastTime = Misc::Sleeper::getTimeMilliseconds();
    quit = false;
    while(!quit) {
        for(;;) {
            Connection::Socket *socket = server.checkForConnections();
            if(!socket) break;
            
            Network::PacketSerializer packetSerializer;
            Network::Packet *packet = new Network::HandshakePacket(clientCount);
            
            Network::StringSerializer stringSerializer(socket);
            stringSerializer.sendString(
                packetSerializer.packetToString(packet));
            
            clients.addClient(socket);
            worldManager->getPlayerList()->addPlayer(
                new Object::Player(clientCount, INITIAL_CAR_LOCATION));
            
            clientCount ++;
        }
        
        {
            Network::Packet *packet;
            while((packet = clients.nextPacket(&whichSocket))) {
                /*LOG(NETWORK, "Packet received from "
                    << whichSocket << ": " << packet);*/
                
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
        
        paintSubsystem->doStep(Misc::Sleeper::getTimeMilliseconds());
        suspension.applySuspension(worldManager->getPlayerList(), NULL);
        
        if(++loops == 5) {
            loops = 0;
            
            /*for(int p = 0; p < clientCount; p ++) {
                LOG(PHYSICS, "Player " << p << " is at "
                    << getPlayerList().getPlayer(p)->getPosition());
            }*/
            
            Event::UpdatePlayerList *update
                = new Event::UpdatePlayerList(
                    Misc::Sleeper::getTimeMilliseconds(),
                    worldManager->getPlayerList());
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
    
    delete physicsWorld;
}

}  // namespace Server
}  // namespace Project
