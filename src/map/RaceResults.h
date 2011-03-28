#ifndef PROJECT_MAP__RACE_RESULTS_H
#define PROJECT_MAP__RACE_RESULTS_H

#include "Teams.h"
#include <map>
#include <vector>

#include "boost/serialization/access.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/vector.hpp"

#include "object/Player.h"
#include "config.h"

namespace Project {
namespace Map {

class RaceResults {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & teamPoints;
        ar & playerRank;
        ar & playerPoints;
        ar & winningTeam;
    }
private:
	int teamPoints [Teams::MAX_TEAMS];

    std::vector<int> playerRank;
	std::map<int, int> playerPoints;
	int winningTeam;

	int calcScore(unsigned int rank) const;
public:
    RaceResults() {}
	RaceResults(const std::vector<Object::Player*>& player_rankings);
	int getTeamPoints(int team_id) const;
	int getPlayerPoints(int player) const;
	int getWinningTeamID() const;
    int getPlayerByRank(int rank) const;
    int getRanks() const;
};

}  // namespace Map
}  // namespace Project

#endif
