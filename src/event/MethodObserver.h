#ifndef PROJECT_EVENT__METHOD_OBSERVER_H
#define PROJECT_EVENT__METHOD_OBSERVER_H

#include "TypedObserver.h"

namespace Project {
namespace Event {

template <typename SuperType, typename EventType>
class MethodObserver : public TypedObserver<EventType> {
public:
    typedef void (SuperType::*method_t)(EventType *);
private:
    SuperType *super;
    method_t method;
public:
    MethodObserver(SuperType *super, method_t method)
        : super(super), method(method) {}
    
    virtual void observe(EventType *event);
};

template <typename SuperType, typename EventType>
void MethodObserver<SuperType, EventType>::observe(EventType *event) {
    (super->*method)(event);
}

template <typename SuperType, typename EventType>
MethodObserver<SuperType, EventType> *makeMethodObserver(SuperType *super,
    void (SuperType::*method)(EventType *)) {
    
    return new MethodObserver<SuperType, EventType>(super, method);
}

}  // namespace Event
}  // namespace Project

#endif
