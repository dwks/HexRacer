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
        ar & player;
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
    int player;
    MovementType movementType;
    double value;
public:
    PlayerAction() : player(0), movementType(ACCELERATE), value(0.0) {}
    PlayerAction(int player, MovementType movementType, double value)
        : player(player), movementType(movementType), value(value) {}
    
    int getPlayer() const { return player; }
    MovementType getMovementType() const { return movementType; }
    double getValue() const { return value; }
    
    virtual EventType::type_t getType() const
        { return EventType::PLAYER_ACTION; }
};

}  // namespace Event
}  // namespace Project

#endif
