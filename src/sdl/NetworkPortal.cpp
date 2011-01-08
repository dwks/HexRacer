#include <typeinfo>

#include "NetworkPortal.h"

#include "log/Logger.h"

#include "connection/ConstructedTCPSocket.h"

#include "network/Packet.h"
#include "network/PacketSerializer.h"

namespace Project {
namespace SDL {

NetworkPortal::NetworkPortal() {
    socket = NULL;
    stringSerializer = NULL;
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
        
        LOG(NETWORK, "Received packet " << typeid(*packet).name());
    }
}

}  // namespace SDL
}  // namespace Project
