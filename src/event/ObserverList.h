#ifndef PROJECT_EVENT__OBSERVER_LIST_H
#define PROJECT_EVENT__OBSERVER_LIST_H

#include <vector>

#include "boost/shared_ptr.hpp"

#include "EventBase.h"
#include "EventType.h"

#include "Observer.h"
#include "TypedObserver.h"
#include "MultiObserver.h"

namespace Project {
namespace Event {

/** Maintains a list of observers and notifies the appropriate ones given any
    event. The observers are owned by this class and automatically freed by
    destroyObject().
    
    This is a singleton class. Observers and Events passed to this class are
    generally automatically freed (unless otherwise specified).
*/
class ObserverList /*: public Singleton...*/ {
private:
    static ObserverList instance;
public:
    static ObserverList &getInstance() { return instance; }
private:
    typedef std::vector<boost::shared_ptr<Observer> > observer_list_t;
    typedef std::vector<observer_list_t> type_list_t;
    type_list_t type_list;
public:
    ObserverList();
    
    void destroyObject();
    
    /** Adds a TypedObserver to this observer list. A TypedObserver is only
        interested in events of type @c Type.
        
        Note: ownership of @a observer is taken over by this class, which will
        automatically free it when necessary.
    */
    template <typename Type>
    void addObserver(TypedObserver<Type> *observer);
    
    /** Adds a MultiObserver to this observer list. A MultiObserver may be
        interested in multiple types of events; @a observer is queried for the
        types it is interested in.
        
        Note: ownership of @a observer is taken over by this class, which will
        automatically free it when necessary.
    */
    void addObserver(MultiObserver *observer);
    
    /** Notifies all interested observers of @a event. The pointer @a event is
        not freed by ObserverList if @a freeEvent is false.
    */
    void notifyObservers(EventBase *event, bool freeEvent = false);
};

template <typename Type>
void ObserverList::addObserver(TypedObserver<Type> *observer) {
    type_list[Type().getType()].push_back(
        boost::shared_ptr<Observer>(observer));
}

}  // namespace Event
}  // namespace Project

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

#endif
