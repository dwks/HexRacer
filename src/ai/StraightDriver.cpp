#include "StraightDriver.h"

namespace Project {
namespace AI {

StraightDriver::StraightDriver() {
    intention.setAccel(1.0);
    intention.setPaint(true);
}

const World::PlayerIntention &StraightDriver::getAction() {
    return intention;
}

}  // namespace AI
}  // namespace Project
