#ifndef PROJECT_EVENT__CREATE_OBJECT_H
#define PROJECT_EVENT__CREATE_OBJECT_H

#include "EventBase.h"
#include "object/ObjectBase.h"

namespace Project {
namespace Event {

/** Contains an object to be created and added to the World.
*/
class CreateObject : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & object;
    }
private:
    Object::ObjectBase *object;
    bool propagate;
public:
    CreateObject() : object(NULL), propagate(false) {}
    CreateObject(Object::ObjectBase *object, bool propagate = true)
        : object(object), propagate(propagate) {}
    
    bool getPropagate() const { return propagate; }
    Object::ObjectBase *getObject() { return object; }
    
    virtual EventType::type_t getType() const
        { return EventType::CREATE_OBJECT; }
};

}  // namespace Event
}  // namespace Project

#endif
