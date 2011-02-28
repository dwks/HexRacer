#ifndef PROJECT_SERVER__SERVER_NETWORK_PORTAL_H
#define PROJECT_SERVER__SERVER_NETWORK_PORTAL_H

#include "ClientManager.h"
#include "event/Enabler.h"
#include "event/MultiObserver.h"

namespace Project {
namespace Server {

class ServerNetworkPortal : public Event::Enabler {
private:
    class EventPropagator : public Event::MultiObserver {
    private:
        ServerNetworkPortal *portal;
    public:
        EventPropagator(ServerNetworkPortal *portal) : portal(portal) {}
        
        virtual void observe(Event::EventBase *event);
        virtual bool interestedIn(Event::EventType::type_t type);
    };
private:
    ClientManager *clientManager;
public:
    ServerNetworkPortal(ClientManager *clientManager);
    
    ClientManager *getClientManager() { return clientManager; }
};

}  // namespace Server
}  // namespace Project

#endif
