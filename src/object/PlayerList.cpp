#include "PlayerList.h"

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

}  // namespace Object
}  // namespace Project
