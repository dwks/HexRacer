#ifndef PROJECT_SERVER__SERVER_MAIN_H
#define PROJECT_SERVER__SERVER_MAIN_H

#include "paint/PaintManager.h"
#include "paint/PaintSubsystem.h"

#include "object/WorldManager.h"

#include "network/PacketVisitor.h"
#include "event/MultiObserver.h"

#include "ServerNetworkPortal.h"
#include "timing/AccelControl.h"

#include "map/HRMap.h"
#include "map/RaceManager.h"

namespace Project {
namespace Server {

class ServerMain {
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
    Object::WorldManager *worldManager;
    Paint::PaintManager paintManager;
    Paint::PaintSubsystem *paintSubsystem;
    ServerVisitor visitor;
    ServerNetworkPortal *networkPortal;
    Timing::AccelControl *accelControl;
    Map::HRMap *map;
    Map::RaceManager *raceManager;
public:
    ServerMain();
    ~ServerMain();
    
    void run();
    
    void setQuit() { quit = true; }
    
    int getWhichSocket() const { return whichSocket; }
    Object::World *getWorld() { return worldManager->getWorld(); }
    Object::WorldManager *getWorldManager() { return worldManager; }
    Paint::PaintManager &getPaintManager() { return paintManager; }
};

}  // namespace Server
}  // namespace Project

#endif
