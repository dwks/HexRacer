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
#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"
#include "event/CreateObject.h"
#include "event/UpdateObject.h"
#include "event/UpdateWorld.h"
#include "event/EntireWorld.h"

#include "event/ObserverList.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"
#include "physics/Suspension.h"

#include "mesh/MeshGroup.h"
#include "mesh/MeshLoader.h"
#include "map/MapLoader.h"
#include "map/PathTracker.h"

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
}

void ServerMain::ServerObserver::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::QUIT:
        main->setQuit();
        break;
    case Event::EventType::PLAYER_ACTION: {
        Event::PlayerAction *action
            = dynamic_cast<Event::PlayerAction *>(event);
        Object::Player *player = main->getWorldManager()
            ->getPlayer(main->getWhichSocket());
        
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
        case Event::PlayerAction::FIX_OFF_TRACK: {
            delete player->getPhysicalObject();
            
            if(player->getPathTracker()
                && player->getPathTracker()->getCurrentNode()) {
            
                const Map::PathNode* node = player->getPathTracker()->getCurrentNode();
                
                Math::Point origin = node->getPosition();
                Math::Point direction = (node->getNextNodes()[0]->getPosition() - node->getPosition()).normalized();
                
                origin.setY(origin.getY() + VEHICLE_RESET_Y_OFFSET);
                player->setPhysicalObject(
                    Physics::PhysicsFactory::createPhysicalPlayer(origin, direction));
            }
            else {
                Math::Point origin = main->raceManager->startingPointForPlayer(player->getID());
                Math::Point direction = Math::Point(0.0, 0.0, -1.0);
                
                origin.setY(origin.getY() + VEHICLE_RESET_Y_OFFSET);
                player->setPhysicalObject(
                    Physics::PhysicsFactory::createPhysicalPlayer(origin, direction));
            }
            break;
        }
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
    case Event::EventType::PAUSE_GAME:  // handled by AccelControl
        return false;
    case Event::EventType::CREATE_OBJECT:
    case Event::EventType::DESTROY_OBJECT:
        return false;
    case Event::EventType::PHYSICS_TICK:  // don't care for the moment
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
    accelControl = new Timing::AccelControl();
    Physics::PhysicsWorld *physicsWorld = new Physics::PhysicsWorld();
    
    Connection::ServerManager server;
    ClientManager clients;
    
    server.addServer(GET_SETTING("network.port", 1820));
    
    networkPortal = new ServerNetworkPortal(&clients);
    
    Mesh::MeshLoader *meshLoader = new Mesh::MeshLoader();
    /*meshLoader->loadOBJ("testTerrain", GET_SETTING("map", "models/testterrain.obj"));
    Render::MeshGroup *test_terrain = meshLoader->getModelByName("testTerrain");
    
    Physics::PhysicsWorld::getInstance()->registerRigidBody(
        Physics::PhysicsFactory::createRigidTriMesh(
            test_terrain->getTriangles()));*/
    
    //Instantiate the map
    map = new Map::HRMap();
    if (map->loadMapFile(GET_SETTING("map", "maps/testtrack.hrm"))) {
        LOG(WORLD, "Loaded Map File " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    else {
        LOG(WORLD, "Unable to load map " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    
    ADD_OBSERVER(new ServerObserver(this));
    
    Physics::Suspension *suspension = new Physics::Suspension();
    
    worldManager = new Object::WorldManager();
    paintSubsystem = new Paint::PaintSubsystem(worldManager, &paintManager, 20);
    
    Map::MapLoader().load(map, NULL);
    raceManager = new Map::RaceManager(map);
	boost::shared_ptr<Map::PathManager> pathManager = boost::shared_ptr<Map::PathManager>(
		new Map::PathManager(map->getPathNodes()));

	worldManager->setPathManager(pathManager.get());
    
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
            delete packet;
            
            Math::Point location
                = raceManager->startingPointForPlayer(clientCount);
            
            clients.addClient(socket);
            Object::Player *player = new Object::Player(clientCount, location);
			player->setPathTracker(new Map::PathTracker(*pathManager.get()));
            //worldManager->addPlayer(player);
            
            Event::EntireWorld *entireWorld = new Event::EntireWorld(
                worldManager->getWorld(), worldManager->getPlayerList());
            packet = new Network::EventPacket(entireWorld);
            stringSerializer.sendString(
                packetSerializer.packetToString(packet));
            delete entireWorld;
            delete packet;
            
            EMIT_EVENT(new Event::CreateObject(player));
            
            clientCount ++;
        }
        
        int disconnected;
        while((disconnected = clients.nextDisconnectedClient()) >= 0) {
            LOG2(NETWORK, CONNECT,
                "Client " << disconnected << " has disconnected");
        }
        
        {
            Network::Packet *packet;
            while((packet = clients.nextPacket(&whichSocket))) {
                /*LOG(NETWORK, "Packet received from "
                    << whichSocket << ": " << packet);*/
                
                packet->accept(visitor);
                delete packet;
            }
        }
        
        paintSubsystem->doStep(Misc::Sleeper::getTimeMilliseconds());
        suspension->setData(worldManager, NULL);
        suspension->checkForWheelsOnGround();
        
        if(!Timing::AccelControl::getInstance()->getPaused()) {
            static unsigned long lastPhysicsTime
                = Misc::Sleeper::getTimeMilliseconds();
            lastPhysicsTime += Timing::AccelControl::getInstance()
                ->getPauseSkip();
            unsigned long thisTime = Misc::Sleeper::getTimeMilliseconds();
            physicsWorld->stepWorld((thisTime - lastPhysicsTime) * 1000);
            lastPhysicsTime = thisTime;
        }
        
        static int loops = 0;
        if(++loops == 5) {
            loops = 0;
            
            Event::UpdateWorld *update
                = new Event::UpdateWorld(
                    Misc::Sleeper::getTimeMilliseconds(),
                    worldManager);
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
        
        accelControl->clearPauseSkip();
    }
    
    delete worldManager;
    delete paintSubsystem;
    
    delete meshLoader;
    delete physicsWorld;
    
    delete accelControl;
}

}  // namespace Server
}  // namespace Project
