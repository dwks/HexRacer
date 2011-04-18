#ifndef PROJECT_EVENT__DO_DISCONNECT_H
#define PROJECT_EVENT__DO_DISCONNECT_H

#include "EventBase.h"

namespace Project {
namespace Event {

class DoDisconnect : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
    }
public:
    DoDisconnect() {}
    
    virtual EventType::type_t getType() const
        { return EventType::DO_DISCONNECT; }
};

}  // namespace Event
}  // namespace Project

#endif
