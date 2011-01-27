#include <sstream>
#include <typeinfo>  // for debugging info

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "HandshakePacket.h"
#include "EventPacket.h"

#include "event/PlayerAction.h"
#include "event/UpdatePlayerList.h"
#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"
#include "event/TogglePainting.h"

#include "PacketSerializer.h"
#include "PointSerializer.h"
#include "log/Logger.h"

namespace Project {
namespace Network {

std::string PacketSerializer::packetToString(Packet *packet) {
    std::ostringstream stream;
    boost::archive::text_oarchive out(stream);
    
    out.register_type<HandshakePacket>();
    out.register_type<EventPacket>();
    
    out.register_type<Event::PlayerAction>();
    out.register_type<Event::UpdatePlayerList>();
    out.register_type<Event::PaintEvent>();
    out.register_type<Event::PaintCellsChanged>();
    out.register_type<Event::TogglePainting>();
    
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
    //LOG(NETWORK, "Parsing packet from \"" << string << "\"");
    
    std::istringstream stream(string);
    boost::archive::text_iarchive in(stream);
    
    in.register_type<HandshakePacket>();
    in.register_type<EventPacket>();
    
    in.register_type<Event::PlayerAction>();
    in.register_type<Event::UpdatePlayerList>();
    in.register_type<Event::PaintEvent>();
    in.register_type<Event::PaintCellsChanged>();
    in.register_type<Event::TogglePainting>();
    
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
