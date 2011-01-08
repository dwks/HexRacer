#ifndef PROJECT_CONNECTION__TCPSOCKET_H
#define PROJECT_CONNECTION__TCPSOCKET_H

#include <QTcpSocket>

#include "Socket.h"

namespace Project {
namespace Connection {

class TCPSocket : public Socket {
private:
    QTcpSocket socket;
public:
    TCPSocket();
    ~TCPSocket();
    
    virtual bool open(const std::string &host, unsigned short port);
    
    virtual void close();
};

}  // namespace Connection
}  // namespace Project

#endif
