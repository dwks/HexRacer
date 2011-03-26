#ifndef PROJECT_MAP__RACE_RESULTS_H
#define PROJECT_MAP__RACE_RESULTS_H

#include "config.h"
#include "object/Player.h"
#include <map>

namespace Project {
namespace Map {

class RaceResults {
private:
	int teamPoints [MAX_TEAMS];
	std::map<Object::Player*, int> playerPoints;
	int winningTeam;

	int calcScore(unsigned int rank) const;
public:
	RaceResults(const std::vector<Object::Player*>& player_rankings);
	int getTeamPoints(int team_id) const;
	int getPlayerPoints(Object::Player* player) const;
	int getWinningTeamID() const;
};

}  // namespace Map
}  // namespace Project

#endif
