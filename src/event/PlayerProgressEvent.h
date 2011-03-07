#ifndef PROJECT_EVENT__PLAYER_PROGRESS_EVENT_H
#define PROJECT_EVENT__PLAYER_PROGRESS_EVENT_H

#include "EventBase.h"

namespace Project {
namespace Event {

/** Indicates the player's progress for the current lap.
*/
class PlayerProgressEvent : public EventBase {
private:
    int lapCount;
    double lapProgress;
public:
    PlayerProgressEvent() {}
    PlayerProgressEvent(int lapCount, double lapProgress)
        : lapCount(lapCount), lapProgress(lapProgress) {}
    
    int getLapCount() const { return lapCount; }
    double getLapProgress() const { return lapProgress; }
    
    virtual EventType::type_t getType() const
        { return EventType::PLAYER_PROGRESS_EVENT; }
};

}  // namespace Event
}  // namespace Project

#endif
