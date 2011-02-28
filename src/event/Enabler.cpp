#include "Enabler.h"
#include "ObserverRegistry.h"

namespace Project {
namespace Event {

void Enabler::addObserver(Observer *observer) {
    addObserver(boost::shared_ptr<Observer>(observer));
}

void Enabler::addObserver(boost::shared_ptr<Observer> observer) {
    observerList.push_back(observer);
    
    ObserverRegistry::getInstance().addObserver(observer);
}

}  // namespace Event
}  // namespace Project
