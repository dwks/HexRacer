#ifndef PROJECT_CONNECTION__TCPSERVER_SOCKET_H
#define PROJECT_CONNECTION__TCPSERVER_SOCKET_H

#include <QTcpServer>

namespace Project {
namespace Connection {

class TCPServerSocket {
private:
    QTcpServer server;
public:
    virtual bool listen(unsigned short port);
};

}  // namespace Connection
}  // namespace Project

#endif
