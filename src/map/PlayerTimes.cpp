#include "PlayerTimes.h"

namespace Project {
namespace Map {

PlayerTimes PlayerTimes::instance;

void PlayerTimes::setFinished(int player, int time) {
    playerTime[player] = time;
}

int PlayerTimes::getFinishTime(int player) {
    return playerTime[player];
}

void PlayerTimes::clear() {
    playerTime.clear();
}

}  // namespace Map
}  // namespace Project
