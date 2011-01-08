#include "ConstructedTCPSocket.h"

#include "log/Logger.h"

namespace Project {
namespace Connection {

ConstructedTCPSocket::ConstructedTCPSocket(
    const char *hostname, unsigned short port)
    : socket(io_service), hostname(hostname), port(port) {
    
}

bool ConstructedTCPSocket::open() {
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(hostname, Misc::StreamAsString() << port);
    
    tcp::resolver::iterator endpoint_it = resolver.resolve(query);
    tcp::resolver::iterator end;
    
    if(endpoint_it == end) return false;
    
    boost::system::error_code error;
    do {
        if(socket.is_open()) {
            socket.close();
        }
        
        socket.connect(*endpoint_it, error);
        
        endpoint_it ++;
    } while(error && endpoint_it != end);
    
    if(error) {
        LOG2(NETWORK, ERROR, "Error opening socket to "
            << hostname << ":" << port << ": " << error);
    }
    
    return !error;
}

void ConstructedTCPSocket::close() {
    socket.close();
}

}  // namespace Connection
}  // namespace Project
