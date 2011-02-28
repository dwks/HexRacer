#ifndef PROJECT_EVENT__UPDATE_WORLD_H
#define PROJECT_EVENT__UPDATE_WORLD_H

#include "EventBase.h"

#include "UpdateObject.h"
#include "boost/serialization/vector.hpp"

#include "object/WorldManager.h"

namespace Project {
namespace Event {

class UpdateWorld : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & milliseconds;
        ar & updates;
    }
private:
    unsigned long milliseconds;
    std::vector<UpdateObject *> updates;
public:
    UpdateWorld() : milliseconds(0) {}
    UpdateWorld(unsigned long milliseconds) : milliseconds(milliseconds) {}
    UpdateWorld(unsigned long milliseconds, Object::WorldManager *worldManager);
    virtual ~UpdateWorld();
    
    /** This object takes ownership of and frees @a update.
    */
    void addUpdate(UpdateObject *update) { updates.push_back(update); }
    
    unsigned long getMilliseconds() const { return milliseconds; }
    
    std::vector<UpdateObject *> &getVector() { return updates; }
    
    virtual EventType::type_t getType() const
        { return EventType::UPDATE_WORLD; }
};

}  // namespace Event
}  // namespace Project

#endif
