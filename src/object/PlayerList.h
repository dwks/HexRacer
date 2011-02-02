#ifndef PROJECT_OBJECT__PLAYER_LIST_H
#define PROJECT_OBJECT__PLAYER_LIST_H

#include <vector>

#include "Player.h"

#include "boost/serialization/vector.hpp"

namespace Project {
namespace Object {

/** Players added to this list are automatically freed when this PlayerList
    is destroyed.
    
    This object should not be accessed by code outside of the Object namespace;
    use WorldManager instead.
*/
class PlayerList {
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & player_list;
    }
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
    ~PlayerList();
    
    void addPlayer(Player *player);
    
    Player *getPlayer(int p);
    int getPlayerCount() { return int(player_list.size()); }
    
    IteratorType getIterator() { return IteratorType(player_list); }
};

}  // namespace Object
}  // namespace Project

#endif
