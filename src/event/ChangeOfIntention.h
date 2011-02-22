#ifndef PROJECT_EVENT__CHANGE_OF_INTENTION_H
#define PROJECT_EVENT__CHANGE_OF_INTENTION_H

#include "EventBase.h"
#include "world/PlayerIntention.h"

namespace Project {
namespace Event {

class ChangeOfIntention : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & propagate;
        ar & player;
        ar & intention;
    }
private:
    bool propagate;
    int player;
    World::PlayerIntention intention;
public:
    ChangeOfIntention() : propagate(false), player(-1) {}
    ChangeOfIntention(int player, const World::PlayerIntention &intention)
        : propagate(true), player(player), intention(intention) {}
    
    void setPropagate(bool p) { propagate = p; }
    bool getPropagate() const { return propagate; }
    
    int getPlayer() const { return player; }
    const World::PlayerIntention &getIntention() const { return intention; }
    
    virtual EventType::type_t getType() const
        { return EventType::CHANGE_OF_INTENTION; }
};

}  // namespace Event
}  // namespace Project

#endif
