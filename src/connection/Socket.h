#ifndef PROJECT_CONNECTION__SOCKET_H
#define PROJECT_CONNECTION__SOCKET_H

#include <string>
#include <cstddef>  // for std::size_t

namespace Project {
namespace Connection {

/** Abstract base class representing a socket connection.
    
    This class is non-copyable.
*/
class Socket {
private:
    Socket(const Socket &other) {}
    void operator = (const Socket &other) {}
public:
    Socket() {}
    virtual ~Socket() {}
    
    /**
        @return True upon success.
    */
    virtual bool open() = 0;
    
    /**
        @return True if this socket is (still) open.
    */
    virtual bool isOpen() = 0;
    
    /** Closes this socket, if it was ever opened. This will automatically be
        called by the destructor.
    */
    virtual void close() = 0;
    
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
