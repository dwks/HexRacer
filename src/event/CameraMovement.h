#ifndef PROJECT_EVENT__CAMERA_MOVEMENT_H
#define PROJECT_EVENT__CAMERA_MOVEMENT_H

#include "EventBase.h"
#include "math/Point.h"

namespace Project {
namespace Event {

/**
    Note: this class is not serializable because it is never sent over the
    network.
*/
class CameraMovement : public EventBase {
private:
    Math::Point movement;
public:
    CameraMovement() {}
    CameraMovement(const Math::Point &movement) : movement(movement) {}
    
    Math::Point getMovement() const { return movement; }
    
    virtual EventType::type_t getType() const
        { return EventType::CAMERA_MOVEMENT; }
};

}  // namespace Event
}  // namespace Project

#endif
