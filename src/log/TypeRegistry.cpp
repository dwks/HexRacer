#include "TypeRegistry.h"

namespace Project {
namespace Log {

//template <>
//TypeRegistry *::Project::Misc::Singleton<TypeRegistry>::instance = 0;
TypeRegistry TypeRegistry::instance;

TypeRegistry::TypeRegistry() {
    // Global log message types
    {
        int counter = 1;
        registerType(Subsystem::GLOBAL, "ERROR", counter ++);
        registerType(Subsystem::GLOBAL, "WARNING", counter ++);
        registerType(Subsystem::GLOBAL, "PROGRESS", counter ++);
        registerType(Subsystem::GLOBAL, "SETTING", counter ++);
    }
    
    // OPENGL log message types
    {
        int counter = 1;
        registerType(Subsystem::OPENGL, "INIT", counter ++);
        registerType(Subsystem::OPENGL, "OPENGL", counter ++);
    }
    
    // SDL log message types
    {
        int counter = 1;
        registerType(Subsystem::SDL, "ERROR", counter ++);
        registerType(Subsystem::SDL, "INPUT", counter ++);
    }
    
    // NETWORK log message types
    {
        int counter = 1;
        registerType(Subsystem::NETWORK, "ERROR", counter ++);
        registerType(Subsystem::NETWORK, "CONNECT", counter ++);
        registerType(Subsystem::NETWORK, "PACKET", counter ++);
    }
    
    // PHYSICS log message types
    {
        int counter = 1;
        registerType(Subsystem::PHYSICS, "INIT", counter ++);
        registerType(Subsystem::PHYSICS, "CREATE", counter ++);
        registerType(Subsystem::PHYSICS, "TIMESTEP", counter ++);
    }
    
    // AUDIO log message types
    {
        int counter = 1;
        registerType(Subsystem::AUDIO, "INIT", counter ++);
        registerType(Subsystem::AUDIO, "MUSIC", counter ++);
    }
    
    // CAMERA OBJECT log message types
    {
        int counter = 1;
        registerType(Subsystem::CAMERA, "INIT", counter ++);
        registerType(Subsystem::CAMERA, "UPDATE", counter ++);    
    }
    
    // World log message types
    {
        int counter = 1;
        registerType(Subsystem::WORLD, "WARNING", counter ++);
    }
    
    // Widget log message types
    {
        int counter = 1;
        registerType(Subsystem::WORLD, "WARNING", counter ++);
    }
}

void TypeRegistry::registerType(Subsystem subsystem,
    const std::string &type, int level) {
    
    type_list[subsystem.getSubsystem()][type] = level;
}

bool TypeRegistry::isValidType(Subsystem subsystem, const std::string &type) {
    type_list_t::iterator found
        = type_list[subsystem.getSubsystem()].find(type);
    
    return found != type_list[subsystem.getSubsystem()].end();
}

int TypeRegistry::getLevelOfType(Subsystem subsystem,
    const std::string &type) {
    
    return type_list[subsystem.getSubsystem()][type];
}

}  // namespace Log
}  // namespace Project
