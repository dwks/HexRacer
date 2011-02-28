#ifndef PROJECT_SDL__NETWORK_PORTAL_H
#define PROJECT_SDL__NETWORK_PORTAL_H

#include "connection/Socket.h"
#include "network/SinglePortal.h"

#include "event/SendPacket.h"
#include "event/TypedObserver.h"
#include "event/MultiObserver.h"
#include "event/Enabler.h"

#include "object/World.h"
#include "object/PlayerList.h"

namespace Project {
namespace SDL {

class NetworkPortal : public Event::Enabler {
private:
    Network::SinglePortal *portal;
    int id;
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
    private:
        void send(Event::EventBase *event);
    };
public:
    NetworkPortal();
    ~NetworkPortal();
    
    int getID() const { return id; }
    
    /** Connects to the server hosted on @a hostname and listening on @a port.
        
        @return True upon success.
    */
    bool connectTo(const char *hostname, unsigned short port);
    
    /** Disconnects from the server.
        
        It is safe to call this function even if connectTo() has not been
        called, or if connectTo() failed.
    */
    void disconnectFrom();
    
    /** Called immediately after a successful connect. Waits for any necessary
        packets from the server.
    */
    void waitForWorld(Object::World *&world, Object::PlayerList *&playerList);
    
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
