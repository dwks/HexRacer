#include <climits>
#include "VerbosityLevel.h"

namespace Project {
namespace Log {

VerbosityLevel::VerbosityLevel() {
    setAllLevels(INT_MAX);
}

VerbosityLevel::VerbosityLevel(int defaultLevel) {
    setAllLevels(defaultLevel);
}

void VerbosityLevel::setAllLevels(int newLevel) {
    for(int x = 0; x < SUBSYSTEMS; x ++) {
        subsystemLevel[x] = newLevel;
    }
}

void VerbosityLevel::setLevel(Subsystem subsystem, int newLevel) {
    subsystemLevel[subsystem.getSubsystem()] = newLevel;
}

int VerbosityLevel::getLevel(Subsystem subsystem) const {
    return subsystemLevel[subsystem.getSubsystem()];
}

int VerbosityLevel::getLevel(int subsystem) const {
    return subsystemLevel[subsystem];
}

bool VerbosityLevel::operator >= (const VerbosityLevel &other) const {
    for(int x = 0; x < SUBSYSTEMS; x ++) {
        if(getLevel(x) < other.getLevel(x)) {
            return false;
        }
    }
    
    return true;
}

}  // namespace Log
}  // namespace Project
