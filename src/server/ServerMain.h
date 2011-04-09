#ifndef PROJECT_SERVER__SERVER_MAIN_H
#define PROJECT_SERVER__SERVER_MAIN_H

#include "boost/smart_ptr.hpp"

#include "world/WorldSetup.h"

#include "paint/PaintManager.h"
#include "paint/PaintSubsystem.h"

#include "object/WorldManager.h"

#include "connection/ServerManager.h"
#include "network/PacketVisitor.h"
#include "event/MultiObserver.h"
#include "event/Enabler.h"

#include "ServerNetworkPortal.h"
#include "timing/AccelControl.h"

#include "map/HRMap.h"
#include "map/MapLoader.h"
#include "map/RaceManager.h"

#include "bonus/GlobalBonusManager.h"

#include "mesh/MeshLoader.h"

#include "world/BasicWorld.h"
#include "ai/AIManager.h"

namespace Project {
namespace Server {

class ServerMain : public Event::Enabler {
private:
    class ServerVisitor : public Network::PacketVisitor {
    private:
        ServerMain *main;
    public:
        ServerVisitor(ServerMain *main) : main(main) {}
        
        virtual void visit(Network::HandshakePacket &packet);
        virtual void visit(Network::EventPacket &packet);
    };
    
    class ServerObserver : public Event::MultiObserver {
    private:
        ServerMain *main;
    public:
        ServerObserver(ServerMain *main) : main(main) {}
        
        virtual void observe(Event::EventBase *event);
        
        virtual bool interestedIn(Event::EventType::type_t type);
    };
private:
    bool quit;
    int clientCount;
    int whichSocket;
    bool loadingStarted, gameStarted;
    
    boost::shared_ptr<Timing::AccelControl> accelControl;
    boost::shared_ptr<World::BasicWorld> basicWorld;
    boost::shared_ptr<Mesh::MeshLoader> meshLoader;
    boost::shared_ptr<Map::HRMap> map;
	boost::shared_ptr<Map::MapLoader> mapLoader;
    
    boost::shared_ptr<Paint::PaintManager> paintManager;
    boost::shared_ptr<Paint::PaintSubsystem> paintSubsystem;
	boost::shared_ptr<Bonus::GlobalBonusManager> bonusManager;
    
    ServerVisitor visitor;
    
    boost::shared_ptr<Connection::ServerManager> server;
    boost::shared_ptr<ClientManager> clients;
    boost::shared_ptr<ServerNetworkPortal> networkPortal;
    
    boost::shared_ptr<AI::AIManager> aiManager;
public:
    ServerMain();
    
    void run();
    
    void setQuit() { quit = true; }
    
    int getWhichSocket() const { return whichSocket; }
    Object::WorldManager *getWorldManager()
        { return basicWorld->getWorldManager(); }
private:
    void initBasics();
    
    void startGame();
    void initMap();
    void initAI();
    void sendWorldToPlayers();
    
    void handleNewConnections();
    void handleDisconnections();
    void handleIncomingPackets();
    void updateClients();
    unsigned long doDelay(unsigned long lastTime);
};

}  // namespace Server
}  // namespace Project

#endif
