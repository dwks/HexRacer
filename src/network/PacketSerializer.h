#ifndef PROJECT_NETWORK__PACKET_SERIALIZER_H
#define PROJECT_NETWORK__PACKET_SERIALIZER_H

#include <string>
#include "Packet.h"

namespace Project {
namespace Network {

/** Transforms between Packets and std::strings (without sizes at the
    beginning).
*/
class PacketSerializer {
public:
    std::string packetToString(Packet *packet);
    Packet *stringToPacket(const std::string &string);
};

}  // namespace Network
}  // namespace Project

#endif
