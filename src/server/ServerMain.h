#ifndef PROJECT_SERVER__SERVER_MAIN_H
#define PROJECT_SERVER__SERVER_MAIN_H

#include "object/PlayerList.h"
#include "paint/PaintManager.h"

#include "network/PacketVisitor.h"
#include "event/MultiObserver.h"

#include "ServerNetworkPortal.h"

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
    Object::PlayerList playerList;
    Paint::PaintManager paintManager;
    ServerVisitor visitor;
    ServerNetworkPortal *networkPortal;
public:
    ServerMain();
    ~ServerMain();
    
    void run();
    
    void setQuit() { quit = true; }
    
    int getWhichSocket() const { return whichSocket; }
    Object::PlayerList &getPlayerList() { return playerList; }
    Paint::PaintManager &getPaintManager() { return paintManager; }
};

}  // namespace Server
}  // namespace Project

#endif
