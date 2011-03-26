#include "TimedSubsystem.h"
#include "AccelControl.h"
#include "settings/SettingsManager.h"

#include "log/Logger.h"

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
    
    /*LOG(SDL, "TimedSubsystem: from " << lastTime << " to "
        << currentTime << ", tick " << tickTime);*/
    
    if(currentTime < lastTime) {
        return;
        lastTime = currentTime;
        timeTakenSoFar = tickTime;
    }
    
    int maxLoops = GET_SETTING("game.timing.maxloops", 3);
    while(timeTakenSoFar >= tickTime && -- maxLoops) {
        lastTime += tickTime;
        timeTakenSoFar -= tickTime;
        
        doAction(lastTime);
    }
    
    if(!maxLoops) {
        lastTime = currentTime;
    }
}

}  // namespace Timing
}  // namespace Project
