#include "PlayerIntention.h"

namespace Project {
namespace World {

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
