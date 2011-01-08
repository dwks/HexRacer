#include "TCPServerSocket.h"

namespace Project {
namespace Connection {

bool TCPServerSocket::listen(unsigned short port) {
    return server.listen(QHostAddress::LocalHost, port);
}

}  // namespace Connection
}  // namespace Project
