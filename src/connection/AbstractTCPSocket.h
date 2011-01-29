#ifndef PROJECT_CONNECTION__ABSTRACT_TCPSOCKET_H
#define PROJECT_CONNECTION__ABSTRACT_TCPSOCKET_H

#include "Socket.h"

#include "boost/asio/ip/tcp.hpp"

namespace Project {
namespace Connection {

using boost::asio::ip::tcp;

class AbstractTCPSocket : public Socket {
private:
    tcp::socket *socket;
    bool shouldFree;
public:
    /** Subclasses must implement this function.
    */
    virtual bool open() = 0;
    
    /** Subclasses must implement this function.
    */
    virtual void close() = 0;
    
    virtual bool isOpen();
    
    virtual bool send(const std::string &message);
    virtual bool tryReceive(std::string &message, std::size_t size);
protected:
    /** Subclasses must implement this function.
    */
    virtual tcp::socket &getSocket() = 0;
};

}  // namespace Connection
}  // namespace Project

#endif
