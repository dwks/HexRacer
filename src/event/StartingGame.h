#ifndef PROJECT_EVENT__STARTING_GAME_H
#define PROJECT_EVENT__STARTING_GAME_H

#include "EventBase.h"

namespace Project {
namespace Event {

class StartingGame : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & status;
    }
public:
    enum GameStatus {
        LOADING_MAP,
        FINISHED_LOADING_MAP,
        COUNTDOWN_STARTED,
        GAME_STARTED
    };
private:
    GameStatus status;
public:
    StartingGame() : status(LOADING_MAP) {}
    StartingGame(GameStatus status) : status(status) {}
    
    virtual GameStatus getStatus() const { return status; }
    
    virtual EventType::type_t getType() const
        { return EventType::STARTING_GAME; }
};

}  // namespace Event
}  // namespace Project

#endif
