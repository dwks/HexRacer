#ifndef PROJECT_EVENT__PLAYER_ACTION_H
#define PROJECT_EVENT__PLAYER_ACTION_H

#include "EventBase.h"

#include "math/Point.h"

namespace Project {
namespace Event {

class PlayerAction : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & movementType;
        ar & value;
    }
public:
    enum MovementType {
        ACCELERATE,
        TURN,
        JUMP,
        FIX_OFF_TRACK
    };
private:
    MovementType movementType;
    double value;
public:
    PlayerAction() {}
    PlayerAction(MovementType movementType, double value)
        : movementType(movementType), value(value) {}
    
    MovementType getMovementType() const { return movementType; }
    double getValue() const { return value; }
    
    virtual EventType::type_t getType() const
        { return EventType::PLAYER_ACTION; }
};

}  // namespace Event
}  // namespace Project

#endif
