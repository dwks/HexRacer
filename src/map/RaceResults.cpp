#include "RaceResults.h"

namespace Project {
namespace Map {

	RaceResults::RaceResults(const std::vector<Object::Player*>& player_rankings) {

		for (int i = 0; i < Map::Teams::MAX_TEAMS; i++) {
			teamPoints[i] = 0;
		}

		for (unsigned int i = 0; i < player_rankings.size(); i++) {
			int score = calcScore(i);
			playerPoints[player_rankings[i]] = score;
			teamPoints[player_rankings[i]->getTeamID()] += score;
		}

		for (int i = 0; i < Map::Teams::MAX_TEAMS; i++) {
			if (i == 0 || teamPoints[i] > teamPoints[winningTeam])
				winningTeam = i;
		}

	}

	int RaceResults::getTeamPoints(int team_id) const {
		return teamPoints[team_id];
	}

	int RaceResults::getPlayerPoints(Object::Player* player) const {
		std::map<Object::Player*, int>::const_iterator it = playerPoints.find(player);
		if (it == playerPoints.end())
			return 0;
		else
			return (*it).second;
	}

	int RaceResults::getWinningTeamID() const {
		return winningTeam;
	}

	int RaceResults::calcScore(unsigned int rank) const {
		return static_cast<int>((100-rank)+(100/(rank+1)));
	}

}  // namespace Map
}  // namespace Project
