#include "SinglePortal.h"
#include "PacketSerializer.h"

namespace Project {
namespace Network {

SinglePortal::SinglePortal(Connection::Socket *socket) {
    this->socket = socket;
    stringSerializer = new StringSerializer(socket);
    // packetSerializer doesn't need initializing
}

SinglePortal::~SinglePortal() {
    delete stringSerializer;
}

void SinglePortal::sendPacket(Packet *packet) {
    // first convert to string, then use StringSerializer
    std::string data = packetSerializer.packetToString(packet);
    stringSerializer->sendString(data);
}

Packet *SinglePortal::nextPacket() {
    // first grab a string with StringSerializer, then convert to a packet
    std::string data;
    if(stringSerializer->readNextString(data)) {
        return packetSerializer.stringToPacket(data);
    }
    else {
        return NULL;
    }
}

}  // namespace Network
}  // namespace Project
