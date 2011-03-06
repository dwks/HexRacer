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
        CHANGE_OF_INTENTION,
        WARP_ONTO_TRACK,
        UPDATE_PLAYER_LIST,
        CAMERA_MOVEMENT,
        SET_DEBUG_DRAWING,
        PAINT_EVENT,
        PAINT_CELLS_CHANGED,
        TOGGLE_PAINT,
        SET_DEBUG_CAMERA,
        PAUSE_GAME,
        CREATE_OBJECT,
        DESTROY_OBJECT,
        UPDATE_OBJECT,
        UPDATE_WORLD,
        ENTIRE_WORLD,
        JOIN_GAME,
        PHYSICS_TICK,
        PHYSICS_COLLISION,
        SWITCH_TO_SCREEN,
        CHANGE_SCREEN_MODE,
        PLAYER_PROGRESS_EVENT,
        TYPES
    };
};

}  // namespace Event
}  // namespace Project

#endif
