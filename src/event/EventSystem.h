#ifndef PROJECT_EVENT__EVENT_SYSTEM_H
#define PROJECT_EVENT__EVENT_SYSTEM_H

#include "ObserverRegistry.h"
#include "MethodObserver.h"

namespace Project {
namespace Event {

/** Adds a method as an observer. The method will persist as an observer until
    this object is destroyed.
    
    The method name passed in must be a pointer to a method for the ``this''
    object, and as such must be fully qualified -- e.g. @c &Class::method.
*/
#define METHOD_OBSERVER(fullyQualifiedMethod) \
    addObserver(Event::makeMethodObserver(this, fullyQualifiedMethod))

/** Adds an Observer object to the Enabler. The Observer should be allocated
    with new, and will be freed automatically.
*/
#define ADD_OBSERVER(observer) \
    ::Project::Event::Enabler::addObserver(observer)

/** Notifies observers of an Event. The event should be dynamically allocated
    with new, and will be automatically freed.
*/
#define EMIT_EVENT(event) \
    ::Project::Event::ObserverRegistry::getInstance() \
        .notifyObservers(event, true)

/** Notifies observers of an Event. This event will not be automatically freed
    if @a free is false.
*/
#define EMIT_EVENT_FREE(event, free) \
    ::Project::Event::ObserverRegistry::getInstance() \
        .notifyObservers(event, free)

}  // namespace Event
}  // namespace Project

#endif
