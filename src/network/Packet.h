#ifndef PROJECT_NETWORK__PACKET_H
#define PROJECT_NETWORK__PACKET_H

#include "PacketVisitor.h"

#include "boost/serialization/access.hpp"
#include "boost/serialization/assume_abstract.hpp"
#include "boost/serialization/base_object.hpp"

namespace Project {
namespace Network {

/** Abstract base class for all packets. Every subclass of Packet must be
serializable using Boost.Serialization.
*/
class Packet {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        // nothing to do
    }
public:
    virtual ~Packet() {}
    
    virtual void accept(PacketVisitor &visitor) = 0;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Packet)

}  // namespace Network
}  // namespace Project

#endif
