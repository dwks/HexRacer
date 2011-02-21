#ifndef PROJECT_EVENT__EVENT_SYSTEM_H
#define PROJECT_EVENT__EVENT_SYSTEM_H

#include "ObserverList.h"

namespace Project {
namespace Event {

/** Macro usually used to add an observer to the ObserverList. The parameter
    passed in should be an Observer allocated with new. The ObserverList will
    free this Observer when necessary.
*/
#define ADD_OBSERVER(observer) \
    ::Project::Event::ObserverList::getInstance().addObserver(observer)

/** Macro usually used to notify observers of an event. The ObserverList will
    free the parameter passed in, which should be a newly allocated pointer to
    an object.
*/
#define EMIT_EVENT(event) \
    ::Project::Event::ObserverList::getInstance().notifyObservers(event, true)

}  // namespace Event
}  // namespace Project

#endif
