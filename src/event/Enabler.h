#ifndef PROJECT_EVENT__ENABLER_H
#define PROJECT_EVENT__ENABLER_H

#include <vector>
#include "boost/smart_ptr/shared_ptr.hpp"

#include "Observer.h"

namespace Project {
namespace Event {

/** Enables the use of Observers in a base class. This class stores pointers to
    Observers to keep them alive until its destruction.
*/
class Enabler {
private:
    typedef std::vector<boost::shared_ptr<Observer> > observerListType;
    observerListType observerList;
public:
    /** Automatically frees all registered Observers.
    */
    virtual ~Enabler() {}
    
    virtual void addObserver(Observer *observer);
    
    virtual void addObserver(boost::shared_ptr<Observer> observer);
};

}  // namespace Event
}  // namespace Project

#endif
