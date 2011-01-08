#ifndef PROJECT_CONNECTION__SOCKET_H
#define PROJECT_CONNECTION__SOCKET_H

#include <string>
#include <cstddef>  // for std::size_t

namespace Project {
namespace Connection {

/** Abstract base class representing a socket connection.
*/
class Socket {
public:
    virtual ~Socket() {}
    
    /**
        @return True upon success.
    */
    virtual bool open() = 0;
    
    /**
        @return True upon success.
    */
    virtual bool send(const std::string &message) = 0;
    
    /** Non-blocking function to attempt to read @a size bytes from the
        network. If there is not enough data available, this function will
        return false.
        
        @return True upon success.
    */
    virtual bool tryReceive(std::string &message, std::size_t size) = 0;
};
}  // namespace Connection
}  // namespace Project

#endif
