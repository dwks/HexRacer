#ifndef PROJECT_EVENT__DESTROY_OBJECT_H
#define PROJECT_EVENT__DESTROY_OBJECT_H

#include "EventBase.h"

namespace Project {
namespace Event {

/** Destroys an object by ID.
*/
class DestroyObject : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & id;
    }
private:
    int id;
public:
    DestroyObject() : id(-1) {}
    DestroyObject(int id) : id(id) {}
    
    int getID() { return id; }
    
    virtual EventType::type_t getType() const
        { return EventType::DESTROY_OBJECT; }
};

}  // namespace Event
}  // namespace Project

#endif
