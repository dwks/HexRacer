#ifndef PROJECT_EVENT__CREATE_OBJECT_H
#define PROJECT_EVENT__CREATE_OBJECT_H

#include "EventBase.h"
#include "object/ObjectBase.h"

namespace Project {
namespace Event {

/** Contains an object to be created and added to the World.
*/
class CreateObject {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & object;
    }
private:
    Object::ObjectBase *object;
public:
    CreateObject() : object(NULL) {}
    CreateObject(Object::ObjectBase *object) : object(object) {}
    
    Object::ObjectBase *getObject() { return object; }
    
    virtual EventType::type_t getType() const
        { return EventType::CREATE_OBJECT; }
};

}  // namespace Event
}  // namespace Project

#endif
