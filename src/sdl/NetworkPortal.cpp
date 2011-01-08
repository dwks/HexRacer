#include <typeinfo>

#include "NetworkPortal.h"

#include "log/Logger.h"

#include "connection/ConstructedTCPSocket.h"

#include "network/Packet.h"
#include "network/PacketSerializer.h"

#include "event/ObserverList.h"
#include "event/PacketReceived.h"

namespace Project {
namespace SDL {

void NetworkPortal::PacketSender::observe(Event::SendPacket *packet) {
    if(portal->getSocket() == NULL) return;
    
    Network::PacketSerializer packetSerializer;
    Network::StringSerializer stringSerializer(portal->getSocket());
    
    std::string data = packetSerializer.packetToString(packet->getPacket());
    stringSerializer.sendString(data);
}

NetworkPortal::NetworkPortal() {
    socket = NULL;
    stringSerializer = NULL;
    
    Event::ObserverList::getInstance().addObserver(new PacketSender(this));
}

NetworkPortal::~NetworkPortal() {
    disconnectFrom();
}

void NetworkPortal::connectTo(const char *hostname, unsigned short port) {
    socket = new Connection::ConstructedTCPSocket(hostname, port);
    if(socket->open()) {
        LOG2(NETWORK, CONNECT, "Successfully connected to server "
            << hostname << ":" << port);
        
        stringSerializer = new Network::StringSerializer(socket);
    }
    else {
        LOG2(NETWORK, CONNECT, "Failed to connect to server "
            << hostname << ":" << port);
        delete socket;
        socket = NULL;
        stringSerializer = NULL;
    }
}

void NetworkPortal::disconnectFrom() {
    delete socket;  // works even if socket is NULL
    delete stringSerializer;
}

void NetworkPortal::checkNetwork() {
    if(!socket) return;
    
    std::string data;
    if(stringSerializer->readNextString(data)) {
        Network::PacketSerializer packetSerializer;
        Network::Packet *packet = packetSerializer.stringToPacket(data);
        
        LOG2(NETWORK, PACKET, "Received packet " << typeid(*packet).name());
        
        Event::EventBase *event = new Event::PacketReceived(packet);
        Event::ObserverList::getInstance().notifyObservers(event);
        
        delete event;
        delete packet;
    }
}

}  // namespace SDL
}  // namespace Project
