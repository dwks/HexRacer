#include "ObserverList.h"

namespace Project {
namespace Event {

ObserverList::ObserverList() : type_list(int(EventType::TYPES)) {
    
}

void ObserverList::addObserver(MultiObserver *observer) {
    for(int type = 0; type < int(EventType::TYPES); type ++) {
        if(observer->interestedIn(static_cast<EventType::type_t>(type))) {
            type_list[type].push_back(observer);
        }
    }
}

void ObserverList::notifyObservers(EventBase *event) {
    observer_list_t &list = type_list[event->getType()];
    
    for(observer_list_t::iterator i = list.begin(); i != list.end(); ++ i) {
        (*i)->observe(event);
    }
}

}  // namespace Event
}  // namespace Project
