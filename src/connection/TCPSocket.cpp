#include "TCPSocket.h"

namespace Project {
namespace Connection {

TCPSocket::TCPSocket() {
    connect(&socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

TCPSocket::~TCPSocket() {
    close();
}

bool TCPSocket::open(const std::string &host, unsigned short port) {
    socket.connectToHost(QString::fromStdString(host), port);
    
    return true;
}

void TCPSocket::close() {
    socket.close();
}

}  // namespace Connection
}  // namespace Project
