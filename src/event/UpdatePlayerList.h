#ifndef PROJECT_EVENT__UPDATE_PLAYER_LIST_H
#define PROJECT_EVENT__UPDATE_PLAYER_LIST_H

#include "EventBase.h"
#include "object/PlayerList.h"

namespace Project {
namespace Event {

/** An event that contains an updated PlayerList.
*/
class UpdatePlayerList : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & timestamp;
        ar & playerList;
    }
private:
    unsigned long timestamp;
    Object::PlayerList *playerList;
public:
    UpdatePlayerList() {}
    UpdatePlayerList(unsigned long timestamp, Object::PlayerList *playerList)
        : timestamp(timestamp), playerList(playerList) {}
    
    unsigned long getTimestamp() const { return timestamp; }
    Object::PlayerList *getPlayerList() const { return playerList; }
    
    virtual EventType::type_t getType() const
        { return EventType::UPDATE_PLAYER_LIST; }
};

}  // namespace Event
}  // namespace Project

#endif
