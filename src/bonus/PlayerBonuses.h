#ifndef PROJECT_BONUS__PLAYER_BONUSES_H
#define PROJECT_BONUS__PLAYER_BONUSES_H

#include <string>

namespace Project {
namespace Bonus {

class PlayerBonuses {
private:

	int playerID;
	int totalBonus;

	int paintedCells;
	int nextPaintBonus;

	int erasedCells;
	int nextEraseBonus;

	bool playerBoosting;
	unsigned long boostStartTime;
	unsigned long boostNextBonusTime;

	void newBonus(int points, const std::string& name);
	
public:
	PlayerBonuses(int playerID = 0);

	void playerFinish(int rank);
	void playerPaint(int cells);
	void playerErase(int cells);
	void playerBoostUpdate(double boost);

	int getTotalBonus() const { return totalBonus; }
};

}  // namespace Bonus
}  // namespace Project

#endif
