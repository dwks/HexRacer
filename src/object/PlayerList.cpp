#include "PlayerList.h"
#include "log/Logger.h"

namespace Project {
namespace Object {

PlayerList::~PlayerList() {
    for(player_list_t::iterator i = player_list.begin();
        i != player_list.end(); ++ i) {
        
        delete (*i);
    }
}

void PlayerList::addPlayer(Player *player) {
    player_list.push_back(player);
}

Player *PlayerList::getPlayer(int p) {
    for(player_list_t::size_type x = 0; x < player_list.size(); x ++) {
        if(player_list[x]->getID() == p) {
            return player_list[x];
        }
    }
    
    LOG2(WORLD, WARNING, "Requested player " << p << " does not exist");
    
    return NULL;
}

}  // namespace Object
}  // namespace Project
