#ifndef PROJECT_EVENT__GAME_STAGE_CHANGED_H
#define PROJECT_EVENT__GAME_STAGE_CHANGED_H

#include "EventBase.h"
#include "world/WorldSetup.h"

namespace Project {
namespace Event {

class GameStageChanged : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & stage;
    }
private:
    World::WorldSetup::GameStage stage;
public:
    GameStageChanged() : stage(World::WorldSetup::AT_LOBBY) {}
    GameStageChanged(World::WorldSetup::GameStage stage) : stage(stage) {}
    
    virtual World::WorldSetup::GameStage getStage() const { return stage; }
    
    virtual EventType::type_t getType() const
        { return EventType::GAME_STAGE_CHANGED; }
};

}  // namespace Event
}  // namespace Project

#endif
