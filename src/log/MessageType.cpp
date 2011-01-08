#include "MessageType.h"
#include "VerbosityLevel.h"
#include "TypeRegistry.h"

namespace Project {
namespace Log {

MessageType::MessageType(Subsystem subsystem)
    : subsystem(subsystem), type() {
    
}

MessageType::MessageType(Subsystem subsystem, const std::string &type)
    : subsystem(subsystem), type(type) {
    
}

Subsystem MessageType::getSubsystem() const {
    return subsystem;
}

VerbosityLevel MessageType::getLevel() const {
    VerbosityLevel level = VerbosityLevel(0);
    int typeLevel = TypeRegistry::getInstance()
        .getLevelOfType(subsystem, type);
    level.setLevel(subsystem, typeLevel);
    
    return level;
}

std::string MessageType::getDescription() const {
    return subsystem.getDescription(type);
}

}  // namespace Log
}  // namespace Project
