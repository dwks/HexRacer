#ifndef PROJECT_EVENT__TYPED_OBSERVER_H
#define PROJECT_EVENT__TYPED_OBSERVER_H

#include "Observer.h"

namespace Project {
namespace Event {

/** An observer for a specific type of event. The event will be automatically
    downcasted to the given Type. If the event is actually of some other type
    (usually indicating a mis-registered Observer) the event will simply be
    ignored.
    
    This implementation currently uses a dynamic_cast, but if it is assumed
    that a TypedObserver will never be registered with the wrong type, then
    a static_cast can be used instead (much more efficiently).
*/
template <typename Type>
class TypedObserver : public Observer {
public:
    virtual void observe(EventBase *event);
    
    /** Subclasses should implement this method.
    */
    virtual void observe(Type *event) = 0;
};

template<typename Type>
void TypedObserver<Type>::observe(EventBase *event) {
    Type *casted = dynamic_cast<Type *>(event);
    if(casted) {
        observe(casted);
    }
}

}  // namespace Event
}  // namespace Project

#endif
