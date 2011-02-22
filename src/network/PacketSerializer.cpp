#include <sstream>
#include <typeinfo>  // for debugging info

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"

#include "HandshakePacket.h"
#include "EventPacket.h"

#include "event/ChangeOfIntention.h"
#include "event/WarpOntoTrack.h"
#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"
#include "event/TogglePainting.h"
#include "event/PauseGame.h"
#include "event/CreateObject.h"
#include "event/DestroyObject.h"
#include "event/UpdateObject.h"
#include "event/UpdateWorld.h"
#include "event/EntireWorld.h"

#include "object/Player.h"
#include "world/PlayerIntention.h"

#include "PacketSerializer.h"
#include "PointSerializer.h"
#include "log/Logger.h"

namespace Project {
namespace Network {

std::string PacketSerializer::packetToString(Packet *packet) {
    std::ostringstream stream;
    boost::archive::binary_oarchive out(stream);
    
    out.register_type<HandshakePacket>();
    out.register_type<EventPacket>();
    
    out.register_type<Event::ChangeOfIntention>();
    out.register_type<Event::WarpOntoTrack>();
    out.register_type<Event::PaintEvent>();
    out.register_type<Event::PaintCellsChanged>();
    out.register_type<Event::TogglePainting>();
    out.register_type<Event::PauseGame>();
    out.register_type<Event::CreateObject>();
    out.register_type<Event::DestroyObject>();
    out.register_type<Event::UpdateObject>();
    out.register_type<Event::UpdateWorld>();
    out.register_type<Event::EntireWorld>();
    
    out.register_type<Object::Player>();
    out.register_type<World::PlayerIntention>();
    
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
    boost::archive::binary_iarchive in(stream);
    
    in.register_type<HandshakePacket>();
    in.register_type<EventPacket>();
    
    in.register_type<Event::ChangeOfIntention>();
    in.register_type<Event::WarpOntoTrack>();
    in.register_type<Event::PaintEvent>();
    in.register_type<Event::PaintCellsChanged>();
    in.register_type<Event::TogglePainting>();
    in.register_type<Event::PauseGame>();
    in.register_type<Event::CreateObject>();
    in.register_type<Event::DestroyObject>();
    in.register_type<Event::UpdateObject>();
    in.register_type<Event::UpdateWorld>();
    in.register_type<Event::EntireWorld>();
    
    in.register_type<Object::Player>();
    in.register_type<World::PlayerIntention>();
    
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
