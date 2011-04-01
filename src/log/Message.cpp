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
#ifdef WIN32
		char *then = new char[32];
		ctime_s(then, 32, &timestamp);
#else
        const char *then = std::ctime(&timestamp);
#endif
        
        // removing trailing newline
        std::string when = std::string(then).substr(0, std::strlen(then) - 1);
#ifdef WIN32
		delete[] then;
#endif
        
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
