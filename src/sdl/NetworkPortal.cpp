#include <typeinfo>

#include "NetworkPortal.h"
#include "SDL.h"  // for SDL_Delay

#include "log/Logger.h"

#include "connection/ConstructedTCPSocket.h"

#include "network/Packet.h"
#include "network/PacketSerializer.h"
#include "network/EventPacket.h"
#include "network/HandshakePacket.h"

#include "event/ObserverList.h"
#include "event/PacketReceived.h"
#include "event/TogglePainting.h"
#include "event/EntireWorld.h"

namespace Project {
namespace SDL {

void NetworkPortal::PacketSender::observe(Event::SendPacket *packet) {
    if(portal->getPortal() == NULL) return;
    
    portal->getPortal()->sendPacket(packet->getPacket());
}

void NetworkPortal::EventPropagator::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::TOGGLE_PAINT: {
        Event::TogglePainting *toggle
            = dynamic_cast<Event::TogglePainting *>(event);
        if(!toggle->getPropagate()) break;
        toggle->setPropagate(false);
        // fall-through
    }
    case Event::EventType::PLAYER_ACTION:
    case Event::EventType::PAUSE_GAME:
    {
        if(portal->getPortal() == NULL) break;
        
        Network::Packet *packet = new Network::EventPacket(event);
        portal->getPortal()->sendPacket(packet);
        delete packet;
        
        break;
    }
    case Event::EventType::PACKET_RECEIVED:
        break;
    default:
        /*LOG2(NETWORK, PACKET, "EventPropagator: Not propagating "
            << typeid(*event).name());*/
        break;
    }
}

bool NetworkPortal::EventPropagator::interestedIn(
    Event::EventType::type_t type) {
    
    return true;  // !!! doesn't hurt to be over-notified right now
}

NetworkPortal::NetworkPortal() {
    portal = NULL;
    id = -1;
    
    ADD_OBSERVER(new PacketSender(this));
    ADD_OBSERVER(new EventPropagator(this));
}

NetworkPortal::~NetworkPortal() {
    disconnectFrom();
}

bool NetworkPortal::connectTo(const char *hostname, unsigned short port) {
    Connection::Socket *socket
        = new Connection::ConstructedTCPSocket(hostname, port);
    if(socket->open()) {
        LOG2(NETWORK, CONNECT, "Successfully connected to server "
            << hostname << ":" << port);
        
        portal = new Network::SinglePortal(socket);
        
        return true;
    }
    else {
        LOG2(NETWORK, CONNECT, "Failed to connect to server "
            << hostname << ":" << port);
        delete socket;
        
        portal = NULL;
        
        return false;
    }
}

void NetworkPortal::disconnectFrom() {
    delete portal;  // works even if portal is NULL
    portal = NULL;
}

void NetworkPortal::waitForWorld(Object::World *&world,
    Object::PlayerList *&playerList) {
    
    LOG2(NETWORK, CONNECT, "Waiting for HandshakePacket . . .");
    
    for(;;) {
        Network::Packet *packet = portal->nextPacket();
        if(packet) {
            Network::HandshakePacket *handshake
                = dynamic_cast<Network::HandshakePacket *>(packet);
            id = handshake->getClientID();
            delete packet;
            break;
        }
        
        SDL_Delay(10);
    }
    
    LOG2(NETWORK, CONNECT, "Got HandshakePacket with ID " << id);
    LOG2(NETWORK, CONNECT, "Waiting for EntireWorld . . .");
    
    for(;;) {
        Network::Packet *packet = portal->nextPacket();
        if(packet) {
            Network::EventPacket *event
                = dynamic_cast<Network::EventPacket *>(packet);
            Event::EntireWorld *entireWorld
                = dynamic_cast<Event::EntireWorld *>(event->getEvent());
            world = entireWorld->getWorld();
            playerList = entireWorld->getPlayerList();
            delete entireWorld;
            delete packet;
            
            break;
        }
        
        SDL_Delay(10);
    }
    
    LOG2(NETWORK, CONNECT, "Got EntireWorld from server");
}

void NetworkPortal::checkNetwork() {
    if(!portal) return;
    
    Network::Packet *packet = portal->nextPacket();
    if(packet) {
        //LOG2(NETWORK, PACKET, "Received packet " << typeid(*packet).name());
        
        if(dynamic_cast<Network::EventPacket *>(packet)) {
            EMIT_EVENT(dynamic_cast<Network::EventPacket *>(packet)
                ->getEvent());
        }
        
        EMIT_EVENT(new Event::PacketReceived(packet));
        delete packet;
    }
}

}  // namespace SDL
}  // namespace Project
