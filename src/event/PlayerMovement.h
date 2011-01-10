#ifndef PROJECT_EVENT__PLAYER_MOVEMENT_H
#define PROJECT_EVENT__PLAYER_MOVEMENT_H

#include "EventBase.h"

#include "math/Point.h"

namespace Project {
namespace Event {

class PlayerMovement : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & movement;
    }
private:
    Math::Point movement;
public:
    PlayerMovement() {}
    PlayerMovement(Math::Point movement) : movement(movement) {}
    
    Math::Point getMovement() const { return movement; }
    
    virtual EventType::type_t getType() const
        { return EventType::PLAYER_MOVEMENT; }
};

}  // namespace Event
}  // namespace Project

#endif
