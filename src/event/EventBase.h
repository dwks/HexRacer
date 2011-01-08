#ifndef PROJECT_EVENT__EVENT_BASE_H
#define PROJECT_EVENT__EVENT_BASE_H

#include "EventType.h"

namespace Project {
namespace Event {

/** A generic event class.
*/
class EventBase {
public:
    virtual ~EventBase() {}
    
    virtual EventType::type_t getType() const = 0;
};

}  // namespace Event
}  // namespace Project

#endif
