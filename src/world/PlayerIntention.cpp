#include "PlayerIntention.h"

namespace Project {
namespace World {

void PlayerIntention::setAccel(double accel) {
    if(accel < -1.0) accel = -1.0;
    else if(accel > +1.0) accel = +1.0;
    
    this->accel = accel;
}

void PlayerIntention::setTurn(double turn) {
    if(turn < -1.0) turn = -1.0;
    else if(turn > +1.0) turn = +1.0;
    
    this->turn = turn;
}

bool PlayerIntention::operator == (const PlayerIntention &other) const {
    return getAccel() == other.getAccel()
        && getTurn() == other.getTurn()
        && getJump() == other.getJump()
        && getReset() == other.getReset()
        && getPaint() == other.getPaint()
        && getErase() == other.getErase();
}

bool PlayerIntention::operator != (const PlayerIntention &other) const {
    return !this->operator == (other);
}

}  // namespace World
}  // namespace Project
