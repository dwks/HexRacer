#ifndef PROJECT_CONNECTION__CONSTRUCTED_TCPSOCKET_H
#define PROJECT_CONNECTION__CONSTRUCTED_TCPSOCKET_H

#include "AbstractTCPSocket.h"
#include "boost/asio/io_service.hpp"

namespace Project {
namespace Connection {

/** Constructs a Boost TCPSocket from a hostname and a port.
*/
class ConstructedTCPSocket : public AbstractTCPSocket {
private:
    boost::asio::io_service io_service;
    tcp::socket socket;
    const char *hostname;
    unsigned short port;
public:
    ConstructedTCPSocket(const char *hostname, unsigned short port);
    ~ConstructedTCPSocket() { close(); }
    
    virtual bool open();
    virtual void close();
protected:
    virtual tcp::socket &getSocket() { return socket; }
private:
    void reportError(boost::system::error_code &error);
};

}  // namespace Connection
}  // namespace Project

#endif
