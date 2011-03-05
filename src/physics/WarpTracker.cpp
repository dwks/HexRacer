#include "WarpTracker.h"

#include "misc/Sleeper.h"
#include "settings/SettingsManager.h"

namespace Project {
namespace Physics {

WarpTracker::WarpTracker() {
    clear();
}

void WarpTracker::setOffTrack(bool current) {
    doCounting(OFF_TRACK, current);
}

void WarpTracker::setSittingStill(bool current) {
    doCounting(SITTING_STILL, current);
}

void WarpTracker::setRequestingHelp(bool current) {
    doCounting(REQUEST_HELP, current);
}

bool WarpTracker::canReceiveWarp() {
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    unsigned long falloff
        = GET_SETTING("game.warp.falloff.countdown", 1000);
    unsigned long upsidedown
        = GET_SETTING("game.warp.upsidedown.countdown", 1000);
    unsigned long helprequest
        = GET_SETTING("game.warp.helprequest.countdown", 1000);
    
    if(counting[OFF_TRACK] && countingSince[OFF_TRACK] + falloff <= now) {
        return true;
    }
    if(counting[SITTING_STILL] && countingSince[SITTING_STILL] + upsidedown <= now) {
        return true;
    }
    if(counting[REQUEST_HELP] && countingSince[REQUEST_HELP] + helprequest <= now) {
        return true;
    }
    
    return false;
}

void WarpTracker::clear() {
    for(int type = 0; type < int(TYPES); type ++) {
        counting[type] = false;
    }
}

void WarpTracker::doCounting(type_t type, bool current) {
    if(!current) {
        counting[type] = false;
    }
    else {
        if(!counting[type]) {
            counting[type] = true;
            countingSince[type] = Misc::Sleeper::getTimeMilliseconds();
        }
    }
}

}  // namespace Physics
}  // namespace Project
