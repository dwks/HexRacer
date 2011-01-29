#ifndef PROJECT_TIMING__ACCEL_CONTROL_H
#define PROJECT_TIMING__ACCEL_CONTROL_H

#include "event/PauseGame.h"
#include "event/TypedObserver.h"

namespace Project {
namespace Timing {

class AccelControl {
private:
    static AccelControl *instance;
public:
    static AccelControl *getInstance() { return instance; }
private:
    class PauseGameHandler : public Event::TypedObserver<Event::PauseGame> {
    public:
        virtual void observe(Event::PauseGame *pause);
    };
private:
    bool paused;
    unsigned long pauseStart;
    unsigned long pauseSkip;
public:
    AccelControl();
    
    void setPaused(bool paused);
    bool getPaused() const { return paused; }
    
    unsigned long getPauseSkip() const { return pauseSkip; }
    void clearPauseSkip();
};

}  // namespace Timing
}  // namespace Project

#endif
