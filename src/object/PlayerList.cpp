#include "PlayerList.h"
#include "WorldManager.h"
#include "log/Logger.h"

namespace Project {
namespace Object {

Player *PlayerList::IteratorType::next() {
    return worldManager->getPlayer(*it ++);
}

void PlayerList::addPlayer(Player *player) {
    player_list.push_back(player->getID());
}

/*Player *PlayerList::getPlayer(int p) {
    for(player_list_t::size_type x = 0; x < player_list.size(); x ++) {
        if(player_list[x] == p) {
            return worldManager->getPlayer(player_list[x]);
        }
    }
    
    LOG2(WORLD, WARNING, "Requested player " << p << " does not exist");
    
    return NULL;
}*/

}  // namespace Object
}  // namespace Project
