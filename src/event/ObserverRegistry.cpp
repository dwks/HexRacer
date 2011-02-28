#include "ObserverRegistry.h"
#include "EventType.h"

#include "log/Logger.h"

namespace Project {
namespace Event {

ObserverRegistry ObserverRegistry::instance;

ObserverRegistry::ObserverRegistry() : type_list(int(EventType::TYPES)) {
    
}

void ObserverRegistry::addObserver(boost::shared_ptr<Observer> observer) {
    for(int type = 0; type < int(EventType::TYPES); type ++) {
        if(observer->interestedIn(static_cast<EventType::type_t>(type))) {
            type_list[type].push_back(observer);
        }
    }
}

void ObserverRegistry::notifyObservers(EventBase *event, bool freeEvent) {
    observer_list_t &list = type_list[event->getType()];
    
    //LOG(GLOBAL, typeid(*event).name() << " event");
    
    if(list.size() == 0) {
        LOG2(GLOBAL, WARNING, "Event type has no observers: "
            << typeid(*event).name());
    }
    
    for(observer_list_t::size_type x = 0; x < list.size(); x ++) {
        if(list[x].expired()) {
            list.erase(list.begin() + x);
            LOG(GLOBAL, "Event type " << event->getType()
                << " observer " << x << " removed");
            x --;
        }
        else {
            list[x].lock()->observe(event);
        }
    }
    
    if(freeEvent) delete event;
}

}  // namespace Event
}  // namespace Project
