#ifndef PROJECT_OBJECT__PLAYER_LIST_H
#define PROJECT_OBJECT__PLAYER_LIST_H

#include <vector>

#include "Player.h"

namespace Project {
namespace Object {

class PlayerList {
private:
    typedef std::vector<Player *> player_list_t;
    player_list_t player_list;
public:
    class IteratorType {
    private:
        player_list_t::iterator it, end;
    public:
        IteratorType(player_list_t &player_list)
            : it(player_list.begin()), end(player_list.end()) {}
        
        Player *next() { return *it ++; }
        bool hasNext() const { return it != end; }
    };
public:
    void addPlayer(Player *player);
    
    IteratorType getIterator() { return IteratorType(player_list); }
};

}  // namespace Object
}  // namespace Project

#endif
