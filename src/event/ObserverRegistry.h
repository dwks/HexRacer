#ifndef PROJECT_EVENT__OBSERVER_REGISTRY_H
#define PROJECT_EVENT__OBSERVER_REGISTRY_H

#include <vector>
#include "boost/smart_ptr/weak_ptr.hpp"

#include "Observer.h"

namespace Project {
namespace Event {

/** Maintains a list of observers and notifies the appropriate ones for any
    given event.
    
    Observers are stored as weak_ptrs to shared_ptrs elsewhere, and when an
    Observer is destroyed this class automatically stops passing events to it.
    Events passed to this class are automatically freed (unless otherwise
    specified).
    
    This is a singleton class.
*/
class ObserverRegistry {
private:
    static ObserverRegistry instance;
public:
    static ObserverRegistry &getInstance() { return instance; }
private:
    typedef std::vector<boost::weak_ptr<Observer> > observer_list_t;
    typedef std::vector<observer_list_t> type_list_t;
    type_list_t type_list;
public:
    ObserverRegistry();
    
    /** Adds an Observer to this observer list. The observer is registered to
        receive events of the type it is interested in.
    */
    void addObserver(boost::shared_ptr<Observer> observer);
    
    /** Notifies all interested observers of @a event. The pointer @a event is
        not freed by ObserverRegistry if @a freeEvent is false.
    */
    void notifyObservers(EventBase *event, bool freeEvent);
};

}  // namespace Event
}  // namespace Project

#endif
