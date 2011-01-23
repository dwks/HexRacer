#ifndef PROJECT_EVENT__EVENT_TYPE_H
#define PROJECT_EVENT__EVENT_TYPE_H

namespace Project {
namespace Event {

/** Contains an enumeration of all event types.
    
    EventType itself cannot be instantiated: EventType::type_t should be used.
*/
class EventType {
private:
    EventType() {}
public:
    enum type_t {
        QUIT,
        PACKET_RECEIVED,
        SEND_PACKET,
        PLAYER_ACTION,
        UPDATE_PLAYER_LIST,
        CAMERA_MOVEMENT,
        SET_DEBUG_DRAWING,
        TYPES
    };
};

}  // namespace Event
}  // namespace Project

#endif
