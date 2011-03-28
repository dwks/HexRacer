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
	int totalLaps;
public:
    PlayerProgressEvent() {}
    PlayerProgressEvent(int lapCount, double lapProgress, int totalLaps)
        : lapCount(lapCount), lapProgress(lapProgress), totalLaps(totalLaps) {}
    
    int getLapCount() const { return lapCount; }
    double getLapProgress() const { return lapProgress; }
	int getTotalLaps() const { return totalLaps; }
    
    virtual EventType::type_t getType() const
        { return EventType::PLAYER_PROGRESS_EVENT; }
};

}  // namespace Event
}  // namespace Project

#endif
