#ifndef PROJECT_BONUS__GLOBAL_BONUS_MANAGER_H
#define PROJECT_BONUS__GLOBAL_BONUS_MANAGER_H

#include "PlayerBonuses.h"
#include <map>

namespace Project {
namespace Bonus {

class GlobalBonusManager {
private:
	std::map<int, PlayerBonuses> playerBonuses;
	static GlobalBonusManager* instance;
public:
	static GlobalBonusManager *getInstance() { return instance; }

	GlobalBonusManager();

	PlayerBonuses& getPlayerBonuses(int id);
};

}  // namespace Bonus
}  // namespace Project

#endif
