#ifndef PROJECT_EVENT__WARP_ONTO_TRACK_H
#define PROJECT_EVENT__WARP_ONTO_TRACK_H

#include "EventBase.h"

namespace Project {
namespace Event {

class WarpOntoTrack : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & player;
    }
private:
    int player;
public:
    WarpOntoTrack() : player(0) {}
    WarpOntoTrack(int player) : player(player) {}
    
    int getPlayer() const { return player; }
    
    virtual EventType::type_t getType() const
        { return EventType::WARP_ONTO_TRACK; }
};

}  // namespace Event
}  // namespace Project

#endif
