#include "TimedSubsystem.h"
#include "AccelControl.h"

namespace Project {
namespace Timing {

TimedSubsystem::TimedSubsystem(unsigned long tickTime)
    : tickTime(tickTime), lastTime(0) {
    
}

void TimedSubsystem::setLastTime(unsigned long timeNow) {
    this->lastTime = timeNow;
}

void TimedSubsystem::addSkipTime(unsigned long skipTime) {
    lastTime += skipTime;
}

void TimedSubsystem::doStep(unsigned long currentTime) {
    if(AccelControl::getInstance()->getPaused()) return;
    addSkipTime(AccelControl::getInstance()->getPauseSkip());
    
    unsigned long timeTakenSoFar = currentTime - lastTime;
    
    while(timeTakenSoFar >= tickTime) {
        lastTime += tickTime;
        timeTakenSoFar -= tickTime;
        
        doAction(lastTime);
    }
}

}  // namespace Timing
}  // namespace Project
