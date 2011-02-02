#ifndef PROJECT_OBJECT__PLAYER_LIST_H
#define PROJECT_OBJECT__PLAYER_LIST_H

#include <vector>

#include "Player.h"

#include "boost/serialization/vector.hpp"

namespace Project {
namespace Object {

class WorldManager;

/** This object should not be accessed by code outside of the Object namespace;
    use WorldManager instead.
*/
class PlayerList {
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & player_list;
    }
private:
    typedef std::vector<int> player_list_t;
    player_list_t player_list;
    
    WorldManager *worldManager;
public:
    class IteratorType {
    private:
        WorldManager *worldManager;
        player_list_t::iterator it, end;
    public:
        IteratorType(WorldManager *worldManager, player_list_t &player_list)
            : worldManager(worldManager),
            it(player_list.begin()), end(player_list.end()) {}
        
        Player *next();
        bool hasNext() const { return it != end; }
    };
public:
    PlayerList(WorldManager *worldManager = 0) : worldManager(worldManager) {}
    
    void setWorldManager(WorldManager *worldManager)
        { this->worldManager = worldManager; }
    
    void addPlayer(Player *player);
    
    int getPlayerCount() { return int(player_list.size()); }
    
    IteratorType getIterator()
        { return IteratorType(worldManager, player_list); }
};

}  // namespace Object
}  // namespace Project

#endif
