#include "ServerManager.h"
#include "ReferencingTCPSocket.h"

#include "log/Logger.h"

#include "boost/bind.hpp"
#include "boost/asio.hpp"

namespace Project {
namespace Connection {

using boost::asio::ip::tcp;

void ServerManager::addServer(unsigned short port) {
    LOG2(NETWORK, CONNECT, "Hosting on port " << port);
    
    tcp::endpoint address(tcp::v4(), port);
    boost::asio::ip::tcp::acceptor *acceptor
        = new boost::asio::ip::tcp::acceptor(io_service, address);
    
    registerHandler(acceptor);
    
    atLeastOneRegistered = true;
}

void ServerManager::registerHandler(boost::asio::ip::tcp::acceptor *acceptor) {
    tcp::socket *socket = new tcp::socket(io_service);
    Socket *tcpsocket = new ReferencingTCPSocket(*socket);
    
    acceptor->async_accept(*socket, boost::bind(
        &ServerManager::handleAccept, this, tcpsocket, acceptor,
            boost::asio::placeholders::error));
}

void ServerManager::handleAccept(Socket *socket,
    boost::asio::ip::tcp::acceptor *acceptor,
    const boost::system::error_code &error) {
    
    if(!error) {
        LOG2(NETWORK, CONNECT, "Client has connected with socket " << socket);
        
        latestSocket = socket;
        
        registerHandler(acceptor);
    }
    else {
        LOG2(NETWORK, CONNECT, "Error while listening on socket: " << error);
    }
}

Socket *ServerManager::checkForConnections() {
    // !!! For some reason if poll_one() is called when nothing has been
    // registered with the io_service, all future calls to poll_one() will
    // do nothing. Hence the code to only call poll_one() if at least one
    // socket has been registered.
    
    if(atLeastOneRegistered && io_service.poll_one()) {
        return latestSocket;
    }
    else {
        return NULL;
    }
}

}  // namespace Connection
}  // namespace Project
