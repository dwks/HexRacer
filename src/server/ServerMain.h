#ifndef PROJECT_SERVER__SERVER_MAIN_H
#define PROJECT_SERVER__SERVER_MAIN_H

#include "object/PlayerList.h"

#include "network/PacketVisitor.h"
#include "event/MultiObserver.h"

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
    int clientCount;
    int whichSocket;
    Object::PlayerList playerList;
    ServerVisitor visitor;
public:
    ServerMain() : clientCount(0), visitor(this) {}
    
    void run();
    
    int getWhichSocket() const { return whichSocket; }
    Object::PlayerList &getPlayerList() { return playerList; }
};

}  // namespace Server
}  // namespace Project

#endif
