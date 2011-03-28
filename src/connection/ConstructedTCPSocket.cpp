#include "ConstructedTCPSocket.h"

#include "log/Logger.h"

namespace Project {
namespace Connection {

ConstructedTCPSocket::ConstructedTCPSocket(
    const char *hostname, unsigned short port)
    : socket(io_service), hostname(hostname), port(port) {
    
}

bool ConstructedTCPSocket::open() {
    boost::system::error_code error;
    
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(hostname, Misc::StreamAsString() << port);
    
    tcp::resolver::iterator endpoint_it = resolver.resolve(query, error);
    tcp::resolver::iterator end;
    
    if(error) {
        reportError(error);
        return false;
    }
    
    if(endpoint_it == end) return false;
    
    do {
        if(socket.is_open()) {
            socket.close();
        }
        
        socket.connect(*endpoint_it, error);
        
        endpoint_it ++;
    } while(error && endpoint_it != end);
    
    if(error) reportError(error);
    
    return !error;
}

void ConstructedTCPSocket::close() {
    socket.close();
}

void ConstructedTCPSocket::reportError(boost::system::error_code &error) {
#ifdef HAVE_BOOST_SYSTEM
    LOG2(NETWORK, ERROR, "Error opening socket to "
        << hostname << ":" << port << ": "
        << boost::system::system_error(error).what());
#else
    LOG2(NETWORK, ERROR, "Error opening socket to "
        << hostname << ":" << port << ": "
        << error);
#endif
}

}  // namespace Connection
}  // namespace Project
