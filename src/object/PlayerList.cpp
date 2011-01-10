#include "PlayerList.h"

namespace Project {
namespace Object {

void PlayerList::addPlayer(Player *player) {
    player_list.push_back(player);
}

}  // namespace Object
}  // namespace Project
