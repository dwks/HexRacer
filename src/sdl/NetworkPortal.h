#ifndef PROJECT_SDL__NETWORK_PORTAL_H
#define PROJECT_SDL__NETWORK_PORTAL_H

#include "connection/Socket.h"
#include "network/SinglePortal.h"

#include "event/SendPacket.h"
#include "event/TypedObserver.h"
#include "event/MultiObserver.h"

namespace Project {
namespace SDL {

class NetworkPortal {
private:
    Network::SinglePortal *portal;
private:
    class PacketSender : public Event::TypedObserver<Event::SendPacket> {
    private:
        NetworkPortal *portal;
    public:
        PacketSender(NetworkPortal *portal) : portal(portal) {}
        
        virtual void observe(Event::SendPacket *packet);
    };
    
    class EventPropagator : public Event::MultiObserver {
    private:
        NetworkPortal *portal;
    public:
        EventPropagator(NetworkPortal *portal) : portal(portal) {}
        
        virtual void observe(Event::EventBase *event);
        virtual bool interestedIn(Event::EventType::type_t type);
    };
public:
    NetworkPortal();
    ~NetworkPortal();
    
    /** Connects to the server hosted on @a hostname and listening on @a port.
    */
    void connectTo(const char *hostname, unsigned short port);
    
    /** Disconnects from the server.
        
        It is safe to call this function even if connectTo() has not been
        called, or if connectTo() failed.
    */
    void disconnectFrom();
    
    /** It is safe to call this function even if connectTo() has not been
        called, or if connectTo() failed.
    */
    void checkNetwork();
protected:
    Network::SinglePortal *getPortal() { return portal; }
};

}  // namespace SDL
}  // namespace Project

#endif
