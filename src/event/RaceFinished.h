#ifndef PROJECT_EVENT__RACE_FINISHED_H
#define PROJECT_EVENT__RACE_FINISHED_H

#include "EventBase.h"

#include "map/RaceResults.h"

namespace Project {
namespace Event {

class RaceFinished : public EventBase {
private:
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & boost::serialization::base_object<EventBase>(*this);
        ar & results;
    }
private:
    Map::RaceResults *results;
public:
    RaceFinished() : results(0) {}
    RaceFinished(Map::RaceResults *results) : results(results) {}
    
    Map::RaceResults *getResults() const { return results; }
    
    virtual EventType::type_t getType() const
        { return EventType::RACE_FINISHED; }
};

}  // namespace Event
}  // namespace Project

#endif
