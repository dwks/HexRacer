#ifndef PROJECT_SERVER__SERVER_MAIN_H
#define PROJECT_SERVER__SERVER_MAIN_H

#include "object/PlayerList.h"

namespace Project {
namespace Server {

class ServerMain {
private:
    int clientCount;
    Object::PlayerList playerList;
public:
    ServerMain() : clientCount(0) {}
    
    void run();
};

}  // namespace Server
}  // namespace Project

#endif
