#include <typeinfo>

#include "NetworkPortal.h"
#include "SDL.h"  // for SDL_Delay

#include "log/Logger.h"

#include "connection/ConstructedTCPSocket.h"

#include "network/Packet.h"
#include "network/PacketSerializer.h"
#include "network/EventPacket.h"
#include "network/HandshakePacket.h"

#include "event/EventSystem.h"
#include "event/PacketReceived.h"
#include "event/ChangeOfIntention.h"
#include "event/EntireWorld.h"

#include "history/PingTimeMeasurer.h"
#include "misc/Sleeper.h"

namespace Project {
namespace SDL {

void NetworkPortal::PacketSender::observe(Event::SendPacket *packet) {
    if(portal->getPortal() == NULL) return;
    
    portal->getPortal()->sendPacket(packet->getPacket());
}

void NetworkPortal::EventPropagator::observe(Event::EventBase *event) {
    switch(event->getType()) {
    case Event::EventType::CHANGE_OF_INTENTION: {
        Event::ChangeOfIntention *changeOfIntention
            = dynamic_cast<Event::ChangeOfIntention *>(event);
        if(!changeOfIntention->getPropagate()) break;
        changeOfIntention->setPropagate(false);
        
        send(event);
        break;
    }
    case Event::EventType::WARP_ONTO_TRACK:
    case Event::EventType::PAUSE_GAME:
    {
        send(event);
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

void NetworkPortal::EventPropagator::send(Event::EventBase *event) {
    if(portal->getPortal() == NULL) return;
    
    Network::Packet *packet = new Network::EventPacket(event);
    portal->getPortal()->sendPacket(packet);
    delete packet;
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
            
            Settings::SettingsManager::getInstance()->set(
                "map", handshake->getMap());
            
            unsigned long sent = handshake->getMilliseconds();
            unsigned long now = Misc::Sleeper::getTimeMilliseconds();
            long offset = -long(now - sent);
            History::PingTimeMeasurer::getInstance()->setClockOffset(offset);
            LOG(NETWORK, "ClockOffset set to " << offset);
            
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
    
    for(;;) {
        Network::Packet *packet = portal->nextPacket();
        if(!packet) break;
        
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
