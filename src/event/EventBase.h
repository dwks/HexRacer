#ifndef PROJECT_EVENT__EVENT_BASE_H
#define PROJECT_EVENT__EVENT_BASE_H

#include "EventType.h"

#include "boost/serialization/access.hpp"
#include "boost/serialization/assume_abstract.hpp"
#include "boost/serialization/base_object.hpp"

namespace Project {
namespace Event {

/** A generic event class.
    
    Every event must have a default constructor. If the event is ever to be
    sent over the network, it must also be serializable. However, if the event
    is only used internally then it does not need to be serializable.
*/
class EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        // nothing to do
    }
public:
    virtual ~EventBase() {}
    
    virtual EventType::type_t getType() const = 0;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(EventBase)

}  // namespace Event
}  // namespace Project

#endif
