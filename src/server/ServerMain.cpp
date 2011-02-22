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
#include "event/ChangeOfIntention.h"
#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"
#include "event/CreateObject.h"
#include "event/DestroyObject.h"
#include "event/UpdateObject.h"
#include "event/UpdateWorld.h"
#include "event/EntireWorld.h"

#include "event/EventSystem.h"

#include "physics/PhysicsFactory.h"

#include "mesh/MeshGroup.h"
#include "mesh/MeshLoader.h"
#include "map/MapLoader.h"
#include "map/PathTracker.h"
#include "map/PathingUpdater.h"

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
    case Event::EventType::CREATE_OBJECT: {
        Event::CreateObject *createObject
            = dynamic_cast<Event::CreateObject *>(event);
        
        if(dynamic_cast<Object::Player *>(createObject->getObject())) {
            Object::Player *player
                = dynamic_cast<Object::Player *>(createObject->getObject());
            
            if(true /*Settings::ProgramSettings::getInstance()->isServer()*/) {
                player->setPathTracker(
                    new Map::PathTracker(*main->getWorldManager()->getPathManager()));
            }
            
            main->getWorldManager()->addPlayer(player);
        }
        else {
            main->getWorldManager()->getWorld()->addObject(createObject->getObject());
        }
        break;
    }
    case Event::EventType::DESTROY_OBJECT: {
        Event::DestroyObject *destroyObject
            = dynamic_cast<Event::DestroyObject *>(event);
        
        main->getWorldManager()->getWorld()->removeObject(
            main->getWorldManager()->getWorld()->getObject(destroyObject->getID()));
        
        break;
    }
    case Event::EventType::UPDATE_OBJECT: {
        Event::UpdateObject *updateObject
            = dynamic_cast<Event::UpdateObject *>(event);
        
        Object::ObjectBase *object
            = main->getWorldManager()->getWorld()->getObject(updateObject->getID());
        
        object->getPhysicalObject()->setData(
            updateObject->getTransformation(),
            updateObject->getLinearVelocity(),
            updateObject->getAngularVelocity());
        
        break;
    }
    case Event::EventType::CHANGE_OF_INTENTION: {
        Event::ChangeOfIntention *changeOfIntention
            = dynamic_cast<Event::ChangeOfIntention *>(event);
        
        int id = changeOfIntention->getPlayer();
        Object::Player *player = main->getWorldManager()->getPlayer(id);
        
        player->setIntention(changeOfIntention->getIntention());
        
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
    case Event::EventType::PHYSICS_TICK:  // don't care for the moment
        return false;
    case Event::EventType::WARP_ONTO_TRACK:  // handled by PlayerManager
        return false;
    default:
        break;
    }
    
    return true;
}

ServerMain::ServerMain() : clientCount(0), visitor(this) {
    // install CTRL-C handler
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

void ServerMain::init() {
    accelControl = boost::shared_ptr<Timing::AccelControl>(
        new Timing::AccelControl());
    
    server = boost::shared_ptr<Connection::ServerManager>(
        new Connection::ServerManager());
    clients = boost::shared_ptr<ClientManager>(new ClientManager());
    
    server->addServer(GET_SETTING("network.serverport", 1820));
    
    networkPortal = boost::shared_ptr<ServerNetworkPortal>(
        new ServerNetworkPortal(clients.get()));
    
    // this must execute before MapLoader loads the map
    meshLoader = boost::shared_ptr<Mesh::MeshLoader>(new Mesh::MeshLoader());
    
    //Instantiate the map
    map = boost::shared_ptr<Map::HRMap>(new Map::HRMap());
    if (map->loadMapFile(GET_SETTING("map", "maps/testtrack.hrm"))) {
        LOG(WORLD, "Loaded Map File " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    else {
        LOG(WORLD, "Unable to load map " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    
    ADD_OBSERVER(new ServerObserver(this));
    
    basicWorld = boost::shared_ptr<World::BasicWorld>(new World::BasicWorld());
    basicWorld->constructBeforeConnect();
    basicWorld->constructSkippingConnect();
    
    Map::MapLoader().load(map.get(), NULL);
    basicWorld->constructAfterConnect(map.get());
    
    paintManager = boost::shared_ptr<Paint::PaintManager>(
        new Paint::PaintManager());
    paintManager->setPaintCells(map->getPaintCells());
    
    paintSubsystem = boost::shared_ptr<Paint::PaintSubsystem>(
        new Paint::PaintSubsystem(
            basicWorld->getWorldManager(), paintManager.get(), 20));
}

void ServerMain::initAI() {
    aiManager = boost::shared_ptr<AI::AIManager>(
        new AI::AIManager(
            basicWorld->getRaceManager(),
            basicWorld->getPathManager()));
    
    int aiCount = GET_SETTING("server.aicount", 0);
    aiManager->createAIs(aiCount);
    clientCount += aiCount;
}

void ServerMain::run() {
    init();
    initAI();
    
    unsigned long lastTime = Misc::Sleeper::getTimeMilliseconds();
    quit = false;
    while(!quit) {
        for(;;) {
            Connection::Socket *socket = server->checkForConnections();
            if(!socket) break;
            
            Network::PacketSerializer packetSerializer;
            Network::Packet *packet = new Network::HandshakePacket(
                clientCount, Misc::Sleeper::getTimeMilliseconds());
            
            Network::StringSerializer stringSerializer(socket);
            stringSerializer.sendString(
                packetSerializer.packetToString(packet));
            delete packet;
            
            Math::Point location = basicWorld->getRaceManager()
                ->startingPointForPlayer(clientCount);
            Math::Point direction = basicWorld->getRaceManager()
                ->startingPlayerDirection();
            
            clients->addClient(socket);
            Object::Player *player = new Object::Player(
                clientCount, location, direction);
			player->setPathTracker(new Map::PathTracker(
                *basicWorld->getPathManager()));
            //worldManager->addPlayer(player);
            
            Event::EntireWorld *entireWorld = new Event::EntireWorld(
                getWorldManager()->getWorld(),
                getWorldManager()->getPlayerList());
            packet = new Network::EventPacket(entireWorld);
            stringSerializer.sendString(
                packetSerializer.packetToString(packet));
            delete entireWorld;
            delete packet;
            
            EMIT_EVENT(new Event::CreateObject(player));
            
            clientCount ++;
        }
        
        int disconnected;
        while((disconnected = clients->nextDisconnectedClient()) >= 0) {
            LOG2(NETWORK, CONNECT,
                "Client " << disconnected << " has disconnected");
        }
        
        {
            Network::Packet *packet;
            while((packet = clients->nextPacket(&whichSocket))) {
                /*LOG(NETWORK, "Packet received from "
                    << whichSocket << ": " << packet);*/
                
                packet->accept(visitor);
                delete packet;
            }
        }
        
        paintSubsystem->doStep(Misc::Sleeper::getTimeMilliseconds());
        
        basicWorld->doPhysics();
        basicWorld->doAI();
        
        static int loops = 0;
        if(++loops == 5) {
            loops = 0;
            
            Event::UpdateWorld *update
                = new Event::UpdateWorld(
                    Misc::Sleeper::getTimeMilliseconds(),
                    getWorldManager());
            Network::Packet *packet = new Network::EventPacket(update);
            clients->sendPacket(packet);
            delete packet;
            delete update;
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
}

}  // namespace Server
}  // namespace Project
