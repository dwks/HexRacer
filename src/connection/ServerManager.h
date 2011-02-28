#ifndef PROJECT_CONNECTION__SERVER_MANAGER_H
#define PROJECT_CONNECTION__SERVER_MANAGER_H

#include "Socket.h"
#include "boost/asio/io_service.hpp"
#include "boost/asio/ip/tcp.hpp"

namespace Project {
namespace Connection {

/** Maintains a list of server sockets that the program is listening on. Users
    of this class can call checkForConnections() to find sockets that connect
    to any of the ports being listened on.
*/
class ServerManager {
private:
    boost::asio::io_service io_service;
    Socket *latestSocket;
    bool atLeastOneRegistered;
public:
    ServerManager() : atLeastOneRegistered(false) {}
    
    /** Start listening on the given port (of localhost).
    */
    void addServer(unsigned short port);
    
    /** Returns the next pending Socket connection if there is one, or NULL if
        there are no other new connections as yet. This function may be called
        repeatedly until NULL is returned to get all pending connections.
    */
    Socket *checkForConnections();
private:
    void registerHandler(boost::asio::ip::tcp::acceptor *acceptor);
    
    void handleAccept(Socket *socket,
        boost::asio::ip::tcp::acceptor *acceptor,
        const boost::system::error_code &error);
};

}  // namespace Connection
}  // namespace Project

#endif
