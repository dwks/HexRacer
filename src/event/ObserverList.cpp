#include <typeinfo>
#include "ObserverList.h"

#include "log/Logger.h"

namespace Project {
namespace Event {

ObserverList ObserverList::instance;

ObserverList::ObserverList() : type_list(int(EventType::TYPES)) {
    
}

void ObserverList::destroyObject() {
    /*for(int type = 0; type < int(EventType::TYPES); type ++) {
        observer_list_t &list = type_list[type];
        
        for(observer_list_t::iterator i = list.begin(); i != list.end(); ++ i) {
            delete *i;
        }
        
        list.clear();
    }*/
}

void ObserverList::addObserver(MultiObserver *observer) {
    for(int type = 0; type < int(EventType::TYPES); type ++) {
        if(observer->interestedIn(static_cast<EventType::type_t>(type))) {
            type_list[type].push_back(observer);
        }
    }
}

void ObserverList::notifyObservers(EventBase *event, bool freeEvent) {
    observer_list_t &list = type_list[event->getType()];
    
    if(list.size() == 0) {
        LOG2(GLOBAL, WARNING, "Event type has no observers: "
            << typeid(*event).name());
    }
    
    for(observer_list_t::iterator i = list.begin(); i != list.end(); ++ i) {
        (*i)->observe(event);
    }
    
    if(freeEvent) delete event;
}

}  // namespace Event
}  // namespace Project
