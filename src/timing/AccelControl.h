#ifndef PROJECT_TIMING__ACCEL_CONTROL_H
#define PROJECT_TIMING__ACCEL_CONTROL_H

#include "event/PauseGame.h"
#include "event/Enabler.h"

namespace Project {
namespace Timing {

class AccelControl : public Event::Enabler {
private:
    static AccelControl *instance;
public:
    static AccelControl *getInstance() { return instance; }
protected:
    void pauseGameHandler(Event::PauseGame *pause);
private:
    bool paused;
    unsigned long pauseStart;
    unsigned long pauseSkip;
public:
    AccelControl();
    
    void setPaused(bool paused);
    bool getPaused() const { return paused; }
    void setPauseSkipDirectly(unsigned long pauseSkip)
        { this->pauseSkip = pauseSkip; }
    
    unsigned long getPauseSkip() const { return pauseSkip; }
    void clearPauseSkip();
};

}  // namespace Timing
}  // namespace Project

#endif
