#include "AccelControl.h"
#include "misc/Sleeper.h"

#include "event/EventSystem.h"

namespace Project {
namespace Timing {

AccelControl *AccelControl::instance = NULL;

void AccelControl::pauseGameHandler(Event::PauseGame *pause) {
    setPaused(pause->getPaused());
}

AccelControl::AccelControl() {
	pauseStart = 0;
	pauseSkip = 0;
    paused = false;
    instance = this;
    
    METHOD_OBSERVER(&AccelControl::pauseGameHandler);
}

void AccelControl::setPaused(bool paused) {
    if(paused == this->paused) return;
    
    this->paused = paused;
    
    if(paused) {
        pauseStart = Misc::Sleeper::getTimeMilliseconds();
    }
    else {
        pauseSkip = Misc::Sleeper::getTimeMilliseconds() - pauseStart;
        pauseStart = 0;
    }
}

void AccelControl::clearPauseSkip() {
    pauseSkip = 0;
}

}  // namespace Timing
}  // namespace Project
