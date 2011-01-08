#include <sstream>
#include <typeinfo>  // for debugging info

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "HandshakePacket.h"

#include "PacketSerializer.h"
#include "log/Logger.h"

namespace Project {
namespace Network {
    
std::string PacketSerializer::packetToString(Packet *packet) {
    std::ostringstream stream;
    boost::archive::text_oarchive out(stream);
    
    out.register_type<HandshakePacket>();
    
    try {
        out << packet;
    }
    catch(boost::archive::archive_exception &e) {
        LOG2(NETWORK, ERROR, "packetToString() exception: " << e.what());
        LOG2(NETWORK, ERROR, "    in object " << typeid(packet).name());
    }
    
    return stream.str();
}

Packet *PacketSerializer::stringToPacket(const std::string &string) {
    std::istringstream stream(string);
    boost::archive::text_iarchive in(stream);
    
    in.register_type<HandshakePacket>();
    
    Packet *packet;
    try {
        in >> packet;
    }
    catch(boost::archive::archive_exception &e) {
        LOG2(NETWORK, ERROR, "stringToPacket() exception: " << e.what());
        LOG2(NETWORK, ERROR, "    with string \"" << string << "\"");
    }
    
    return packet;
}

}  // namespace Network
}  // namespace Project
