#include "TimedSubsystem.h"

namespace Project {
namespace Timing {

TimedSubsystem::TimedSubsystem(unsigned long tickTime)
    : tickTime(tickTime), lastTime(0) {
    
}

void TimedSubsystem::setLastTime(unsigned long timeNow) {
    this->lastTime = timeNow;
}

void TimedSubsystem::doStep(unsigned long currentTime) {
    unsigned long timeTakenSoFar = currentTime - lastTime;
    
    while(timeTakenSoFar >= tickTime) {
        lastTime += tickTime;
        timeTakenSoFar -= tickTime;
        
        doAction(lastTime);
    }
}

}  // namespace Timing
}  // namespace Project
