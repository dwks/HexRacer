#include <cstddef>  // for std::size_t
#include "Subsystem.h"
#include "TypeRegistry.h"

namespace Project {
namespace Log {

Subsystem::Subsystem(subsystem_t subsystem) : subsystem(subsystem) {
    
}

std::string Subsystem::getDescription() const {
    static const char *descriptions[] = {
        "GLOBAL",
        "OPENGL",
        "PAINT",
        "SDL",
        "NETWORK",
        "PHYSICS",
        "AUDIO",
        "CAMERA",
        "WORLD"
    };
    int size = static_cast<int>(sizeof(descriptions) / sizeof(*descriptions));
    
    if(subsystem >= 0 && subsystem < size) {
        return descriptions[subsystem];
    }
    else {
        return "???";
    }
}

std::string Subsystem::getDescription(const std::string &type) const {
    if(type.length() == 0){ 
        return getDescription();
    }
    else if(TypeRegistry::getInstance().isValidType(subsystem, type)) {
        return getDescription() + "::" + type;
    }
    else {
        return "***INVALID_TYPE***";
    }
}

}  // namespace Log
}  // namespace Project
