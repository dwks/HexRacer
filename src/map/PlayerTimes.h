#ifndef PROJECT_MAP__PLAYER_TIMES_H
#define PROJECT_MAP__PLAYER_TIMES_H

#include <map>

namespace Project {
namespace Map {

class PlayerTimes {
private:
    static PlayerTimes instance;
public:
    static PlayerTimes &getInstance() { return instance; }
private:
    std::map<int, int> playerTime;
public:
    void setFinished(int player, int time);
    
    int getFinishTime(int player);
    
    void clear();
};

}  // namespace Map
}  // namespace Project

#endif
