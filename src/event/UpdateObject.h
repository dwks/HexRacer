#ifndef PROJECT_EVENT__UPDATE_OBJECT_H
#define PROJECT_EVENT__UPDATE_OBJECT_H

#include "EventBase.h"
#include "math/Point.h"
#include "math/Matrix.h"

class btRigidBody;

namespace Project {
namespace Event {

/** Physical updates to an object.
*/
class UpdateObject : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & transformation;
        ar & linearVelocity;
        ar & angularVelocity;
    }
private:
    int id;
    Math::Matrix transformation;
    Math::Point linearVelocity;
    Math::Point angularVelocity;
public:
    UpdateObject() : id(-1) {}
    UpdateObject(int id, const Math::Matrix &transformation,
        const Math::Point &linearVelocity, const Math::Point &angularVelocity)
        : id(id), transformation(transformation),
        linearVelocity(linearVelocity), angularVelocity(angularVelocity) {}
    
    int getID() const { return id; }
    const Math::Matrix &getTransformation() { return transformation; }
    const Math::Point &getLinearVelocity() { return linearVelocity; }
    const Math::Point &getAngularVelocity() { return angularVelocity; }
    
    virtual EventType::type_t getType() const
        { return EventType::UPDATE_OBJECT; }
};

}  // namespace Event
}  // namespace Project

#endif
