#include <set>

#include "RaceResults.h"
#include "PlayerTimes.h"

#include "bonus/GlobalBonusManager.h"

namespace Project {
namespace Map {

	RaceResults::RaceResults(const std::vector<Object::Player*>& player_rankings, unsigned long total_time) {

		LOG(WORLD, "Calculating race results. Total Time: " << total_time);

		for (int i = 0; i < Map::Teams::MAX_TEAMS; i++) {
			teamPoints[i] = 0;
		}

		int total_laps = player_rankings[0]->getPathTracker()->getNumLaps();
        
        int teamPlayers[Teams::MAX_TEAMS] = {};
        
		playerRank.resize(player_rankings.size());
		for (unsigned int i = 0; i < player_rankings.size(); i++) {

			unsigned long finish_time;

			if (player_rankings[i]->getPathTracker()->getFinished()) {
				finish_time = PlayerTimes::getInstance().getFinishTime(player_rankings[i]->getID());
			}
			else {
				float total_progress = static_cast<float>(total_laps);
				float progress = player_rankings[i]->getPathTracker()->getRaceProgress()/total_progress;
				LOG(WORLD, "Player " << i << " progress: " << progress);
				finish_time = (int) ((float)total_time/progress);

				if (Bonus::GlobalBonusManager::getInstance())
					Bonus::GlobalBonusManager::getInstance()->getPlayerBonuses(player_rankings[i]->getID()).playerFinish(i);
			}
			int bonus = 0;

			if (Bonus::GlobalBonusManager::getInstance())
				bonus = Bonus::GlobalBonusManager::getInstance()->getPlayerBonuses(player_rankings[i]->getID()).getTotalBonus();

			int score = calcScore(finish_time)-bonus;

			LOG(WORLD, "Player " << i << " score: " << score << " bonus: " << bonus << " finish time: " << finish_time);

			playerRank[i] = player_rankings[i]->getID();
			playerPoints[player_rankings[i]->getID()] = score;
			playerBonus[player_rankings[i]->getID()] = bonus;
			teamPoints[player_rankings[i]->getTeamID()] += score;
            teamPlayers[player_rankings[i]->getTeamID()] ++;
		}
        
        std::set<int> remainingTeams;
        
		for(int i = 0; i < Map::Teams::MAX_TEAMS; i++) {
			if(teamPlayers[i] > 0) {
				remainingTeams.insert(i);
                teamPoints[i] /= teamPlayers[i];
            }
		}
        
        while(!remainingTeams.empty()) {
            int minTeam = *remainingTeams.begin();
            int minScore = teamPoints[minTeam];
            for(std::set<int>::iterator i = remainingTeams.begin();
                i != remainingTeams.end(); ++ i) {
                
                if(teamPoints[*i] < minScore) {
                    minScore = teamPoints[*i];
                    minTeam = *i;
                }
            }
            
            teamRank.push_back(minTeam);
            remainingTeams.erase(minTeam);
        }
	}

	int RaceResults::getTeamPoints(int team_id) const {
		return teamPoints[team_id];
	}

	int RaceResults::getPlayerPoints(int player) const {
		std::map<int, int>::const_iterator it = playerPoints.find(player);
        
		if (it == playerPoints.end())
			return 0;
		else
			return (*it).second;
	}

	int RaceResults::getPlayerBonus(int player) const {
		std::map<int, int>::const_iterator it = playerBonus.find(player);
        
		if (it == playerBonus.end())
			return 0;
		else
			return (*it).second;
	}

    int RaceResults::getPlayerByRank(int rank) const {
        return playerRank[rank];
    }
    
    int RaceResults::getRanks() const {
        return static_cast<int>(playerRank.size());
    }

    int RaceResults::getTeamByRank(int rank) const {
        return teamRank[rank];
    }
    
    int RaceResults::getTeams() const {
        return teamRank.size();
    }

	int RaceResults::calcScore(unsigned long finish_time) const {
		return static_cast<int>(finish_time);
	}

}  // namespace Map
}  // namespace Project
