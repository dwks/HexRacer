#ifndef PROJECT_LOG__VERBOSITY_LEVEL_H
#define PROJECT_LOG__VERBOSITY_LEVEL_H

#include "Subsystem.h"

namespace Project {
namespace Log {

class VerbosityLevel {
private:
    static const int SUBSYSTEMS = Subsystem::SUBSYSTEMS;
    int subsystemLevel[SUBSYSTEMS];
public:
    VerbosityLevel();
    VerbosityLevel(int defaultLevel);
    
    void setAllLevels(int newLevel);
    void setLevel(Subsystem subsystem, int newLevel);
    
    int getLevel(Subsystem subsystem) const;
    int getLevel(int subsystem) const;
    int getSubsystems() const { return SUBSYSTEMS; }
    
    bool operator >= (const VerbosityLevel &other) const;
};

}  // namespace Log
}  // namespace Project

#endif
