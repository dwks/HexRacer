#ifndef PROJECT_EVENT__OBSERVER_LIST_H
#define PROJECT_EVENT__OBSERVER_LIST_H

#include <vector>

#include "EventBase.h"
#include "EventType.h"

#include "Observer.h"
#include "TypedObserver.h"
#include "MultiObserver.h"

namespace Project {
namespace Event {

/** Maintains a list of observers and notifies the appropriate ones given any
    event.
    
    This is a singleton class.
*/
class ObserverList /*: public Singleton...*/ {
private:
    static ObserverList instance;
public:
    static ObserverList &getInstance() { return instance; }
private:
    typedef std::vector<Observer *> observer_list_t;
    typedef std::vector<observer_list_t> type_list_t;
    type_list_t type_list;
public:
    ObserverList();
    
    /** Adds a TypedObserver to this observer list. A TypedObserver is only
        interested in events of type @c Type.
    */
    template <typename Type>
    void addObserver(TypedObserver<Type> *observer)
        { type_list[Type().getType()].push_back(observer); }
    
    /** Adds a MultiObserver to this observer list. A MultiObserver may be
        interested in multiple types of events; @a observer is queried for the
        types it is interested in.
    */
    void addObserver(MultiObserver *observer);
    
    /** Notifies all interested observers of @a event.
    */
    void notifyObservers(EventBase *event);
};

}  // namespace Event
}  // namespace Project

#endif
