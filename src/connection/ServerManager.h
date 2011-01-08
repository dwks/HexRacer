#ifndef PROJECT_CONNECTION__SERVER_MANAGER_H
#define PROJECT_CONNECTION__SERVER_MANAGER_H

#include "Socket.h"
#include "boost/asio/io_service.hpp"
#include "boost/asio/ip/tcp.hpp"

namespace Project {
namespace Connection {

class ServerManager {
private:
    boost::asio::io_service io_service;
    Socket *latestSocket;
    bool atLeastOneRegistered;
public:
    ServerManager() : atLeastOneRegistered(false) {}
    
    void addServer(short port);
    
    Socket *checkForConnections();
private:
    void registerHandler(boost::asio::ip::tcp::acceptor *acceptor);
    
    void handleAccept(Connection::Socket *socket,
        boost::asio::ip::tcp::acceptor *acceptor,
        const boost::system::error_code &error);
};

}  // namespace Connection
}  // namespace Project

#endif
