#include "GlobalBonusManager.h"

namespace Project {
namespace Bonus {

	GlobalBonusManager *GlobalBonusManager::instance = NULL;

	GlobalBonusManager::GlobalBonusManager() {
		instance = this;
	}

	PlayerBonuses& GlobalBonusManager::getPlayerBonuses(int id) {
		std::map<int, PlayerBonuses>::iterator it = playerBonuses.find(id);
		if (it != playerBonuses.end())
			return (*it).second;
		else {
			playerBonuses[id] = PlayerBonuses(id);
			return playerBonuses[id];
		}
	}

}  // namespace Bonus
}  // namespace Project
