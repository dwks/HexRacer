#ifndef PROJECT_LOG__SUBSYSTEM_H
#define PROJECT_LOG__SUBSYSTEM_H

#include <string>

namespace Project {
namespace Log {

class Subsystem {
public:
    enum subsystem_t {
        GLOBAL,
        GUI,
        OPENGL,
        SDL,
        NETWORK,
	PHYSICS,
        SUBSYSTEMS
    };
private:
    subsystem_t subsystem;
public:
    Subsystem(subsystem_t subsystem);
    
    subsystem_t getSubsystem() const { return subsystem; }
    
    std::string getDescription() const;
    std::string getDescription(const std::string &type) const;
};

}  // namespace Log
}  // namespace Project

#endif
