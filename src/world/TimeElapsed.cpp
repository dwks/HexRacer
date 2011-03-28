#include "TimeElapsed.h"

#include "misc/Sleeper.h"

namespace Project {
namespace World {

TimeElapsed TimeElapsed::instance;

unsigned long TimeElapsed::getGameTime() const {
    return Misc::Sleeper::getTimeMilliseconds() - startTime;
}

}  // namespace World
}  // namespace Project
