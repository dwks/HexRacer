#include <cstring>
#include <sstream>
#include <iomanip>
#include "Message.h"
#include "misc/StreamAsString.h"

namespace Project {
namespace Log {

Message::Message(MessageType type, const std::string &text)
    : type(type), text(text) {
    
    std::time(&timestamp);
}

std::string Message::asText(bool allData) const {
    Misc::StreamAsString stream;
    
    if(allData) {
        const char *then = std::ctime(&timestamp);
        
        // removing trailing newline
        std::string when = std::string(then).substr(0, std::strlen(then) - 1);
        
        stream << "[" << when << "] ";
    }
    
    std::ostringstream typeStr;
    typeStr << std::left << std::setw(1 + 8 + 2 + 10 + 1)
        << ("[" + type.getDescription() + "]");
    
    stream << typeStr.str() << " " << text;
    
    if(text.length() <= 0 || text[text.length() - 1] != '\n') {
        stream << "\n";
    }
    
    return stream;
}

}  // namespace Log
}  // namespace Project
