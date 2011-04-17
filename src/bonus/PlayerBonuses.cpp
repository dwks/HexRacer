#include "PlayerBonuses.h"
#include "settings/SettingsManager.h"

#include "event/EventSystem.h"
#include "event/BonusEvent.h"

#include "log/Logger.h"

namespace Project {
namespace Bonus {

	PlayerBonuses::PlayerBonuses(int playerID)
		: playerID(playerID) {

		totalBonus = 0;

		paintedCells = 0;
		nextPaintBonus = GET_SETTING("bonus.paintbonus.cellcountincrement", 100);

		erasedCells = 0;
		nextEraseBonus = GET_SETTING("bonus.erasebonus.cellcountincrement", 100);

		playerBoosting = false;
		boostStartTime = 0;
		boostNextBonusTime = 0;

	}

	void PlayerBonuses::playerPaint(int cells) {
		paintedCells += cells;
		while (paintedCells >= nextPaintBonus) {
			newBonus(GET_SETTING("bonus.paintbonus.points", 100), Misc::StreamAsString() << "Painted " << nextPaintBonus << " Cells!" );
			nextPaintBonus += GET_SETTING("bonus.paintbonus.cellcountincrement", 100);
		}
	}

	void PlayerBonuses::playerErase(int cells) {
		erasedCells += cells;
		while (erasedCells >= nextEraseBonus) {
			newBonus(GET_SETTING("bonus.erasebonus.points", 100), Misc::StreamAsString() << "Erased " << nextEraseBonus << " Cells!" );
			nextEraseBonus += GET_SETTING("bonus.erasebonus.cellcountincrement", 100);
		}
	}

	void PlayerBonuses::playerBoostUpdate(double boost) {
		
	}

	void PlayerBonuses::playerFinish(int rank) {
		newBonus(GET_SETTING("bonus.rankbonus.maxbonus", 1000)/(rank+1), Misc::StreamAsString() << "Finished with Rank " << (rank+1));
	}

	void PlayerBonuses::newBonus(int points, const std::string& name) {
		if (GET_SETTING("bonus.enable", true)) {
			totalBonus += points;
			EMIT_EVENT(new Event::BonusEvent(playerID, points, name));
			LOG(BONUS, "Player: " << playerID << " Points: " << points << " Name: " << name);
		}
	}

}  // namespace Bonus
}  // namespace Project
