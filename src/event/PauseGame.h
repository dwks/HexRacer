#ifndef PROJECT_EVENT__PAUSE_GAME_H
#define PROJECT_EVENT__PAUSE_GAME_H

#include "EventBase.h"

namespace Project {
namespace Event {

/** Serializable packet which pauses the game. Normally, any player can resume
    the game from a pause state.
*/
class PauseGame : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & paused;
    }
private:
    bool paused;
public:
    PauseGame() {}
    PauseGame(bool paused) : paused(paused) {}
    
    bool getPaused() const { return paused; }
    
    virtual EventType::type_t getType() const
        { return EventType::PAUSE_GAME; }
};

}  // namespace Event
}  // namespace Project

#endif
