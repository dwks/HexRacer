#include "WarpDetector.h"

#include "event/EventSystem.h"
#include "event/WarpOntoTrack.h"

#include "physics/WarpTracker.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace Map {

void WarpDetector::checkForWarping(Object::Player *player) {
    Physics::WarpTracker *warpTracker
        = player->getPhysicalObject()->getWarpTracker();
    
    // if falling off the track, allow warping
    bool offTrack = !raceManager->inBounds(player->getPosition());
    warpTracker->setOffTrack(offTrack);
    
    // if sitting still for too long, allow warping
    double stillVelocity = GET_SETTING("game.warp.stillvelocity", 0.05);
    double linearVelocity
        = player->getPhysicalObject()->getLinearVelocity().length();
    bool sittingStill = (linearVelocity < stillVelocity);
    
    Math::Point up = player->getPhysicalObject()->getUpDirection();
    bool upsideDown = (up.dotProduct(Math::Point(0.0, 1.0, 0.0)) <= 0.10);
    
    warpTracker->setSittingStill(sittingStill && upsideDown);
    
    // check for help request
    if(player->getIntention().getReset()) {
        warpTracker->setRequestingHelp(true);
    }
    
    // finally emit a warp event if necessary
    if(warpTracker->canReceiveWarp()) {
        EMIT_EVENT(new Event::WarpOntoTrack(player->getID()));
        warpTracker->clear();
    }
}

}  // namespace Map
}  // namespace Project
