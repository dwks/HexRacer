#ifndef PROJECT_LOG__MESSAGE_TYPE_H
#define PROJECT_LOG__MESSAGE_TYPE_H

#include <string>
#include "Subsystem.h"

namespace Project {
namespace Log {

class VerbosityLevel;

class MessageType {
private:
    Subsystem subsystem;
    std::string type;
public:
    MessageType(Subsystem subsystem);
    MessageType(Subsystem subsystem, const std::string &type);
    
    Subsystem getSubsystem() const;
    VerbosityLevel getLevel() const;
    
    std::string getDescription() const;
};

}  // namespace Log
}  // namespace Project

#endif
