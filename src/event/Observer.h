#ifndef PROJECT_EVENT__OBSERVER_H
#define PROJECT_EVENT__OBSERVER_H

#include "EventBase.h"

namespace Project {
namespace Event {

/** Abstract base class for all event observers.
*/
class Observer {
public:
    virtual ~Observer() {}
    
    /** Subclasses should implement this method.
    */
    virtual void observe(EventBase *event) = 0;
    
    /** Subclasses should implement this method. Returns true if this observer
        is interested in being notified of events of type @a type.
    */
    virtual bool interestedIn(EventType::type_t type) = 0;
};

}  // namespace Event
}  // namespace Project

#endif
