#include "AbstractTCPSocket.h"
#include "log/Logger.h"

#include <vector>
#include <algorithm>  // for std::copy
#include "boost/asio.hpp"

namespace Project {
namespace Connection {

bool AbstractTCPSocket::isOpen() {
    return getSocket().is_open();
}

bool AbstractTCPSocket::send(const std::string &message) {
    boost::system::error_code error;
    boost::asio::write(getSocket(),
        boost::asio::buffer(message),
        boost::asio::transfer_all(), error);
    
    return !error;
}

bool AbstractTCPSocket::tryReceive(std::string &message, std::size_t size) {
    //boost::system::error_code error;
    std::size_t available = getSocket().available();
    
    /*if(error) {
        LOG2(NETWORK, ERROR, "TCPSocket::tryReceive() error: " << error);
        return false;
    }*/
    
    // not enough data yet
    if(available < size) {
        return false;
    }
    
    std::vector<char> tempBuffer(size);
    
    boost::system::error_code error;
    boost::asio::read(getSocket(),
        boost::asio::buffer(tempBuffer, size),
        boost::asio::transfer_all(), error);
    
    message.resize(size);
    std::copy(tempBuffer.begin(), tempBuffer.end(), message.begin());
    
    if(error) {
        LOG2(NETWORK, ERROR, "TCPSocket::tryReceive() error: "
            << boost::system::system_error(error).what());
    }
    
    return !error;
}

}  // namespace Connection
}  // namespace Project
