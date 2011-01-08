#include <sstream>
#include <iomanip>  // for std::setw
#include "StringSerializer.h"

#include "log/Logger.h"

namespace Project {
namespace Network {

StringSerializer::StringSerializer(Connection::Socket *socket)
    : socket(socket) {
    
    nextReadSize = -1;
}

bool StringSerializer::readNextString(std::string &data) {
    // first read the packet size from the socket
    if(nextReadSize < 0) {
        std::string sizeString;
        if(socket->tryReceive(sizeString, SIZE_BYTES + 1)) {
            std::istringstream stream(sizeString);
            if(!(stream >> nextReadSize)) {
                nextReadSize = -1;
                LOG2(NETWORK, ERROR, "Cannot deserialize packet size"
                    " from \"" << sizeString << "\"");
            }
        }
        else {
            return false;
        }
    }
    
    // next read the packet itself
    if(socket->tryReceive(data, nextReadSize)) {
        nextReadSize = -1;
        return true;
    }
    else {
        return false;
    }
}

void StringSerializer::sendString(const std::string &data) {
    std::ostringstream stream;
    stream << std::setw(SIZE_BYTES) << data.size() << " ";
    
    socket->send(stream.str());
    socket->send(data);
}

}  // namespace Network
}  // namespace Project
