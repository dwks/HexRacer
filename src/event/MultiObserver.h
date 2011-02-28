#ifndef PROJECT_EVENT__MULTI_OBSERVER_H
#define PROJECT_EVENT__MULTI_OBSERVER_H

#include "Observer.h"

namespace Project {
namespace Event {

/** Simple abstract class for Observers which are interested in more than one
    event type. A typical implementation would perform a switch on the type of
    the event and downcast as appropriate.
    
    Currently this is no different from Observer itself.
*/
class MultiObserver : public Observer {
public:
    virtual void observe(EventBase *event) = 0;
    
    virtual bool interestedIn(EventType::type_t type) = 0;
};

}  // namespace Event
}  // namespace Project

#endif
