#ifndef PROJECT_EVENT__QUIT_EVENT_H
#define PROJECT_EVENT__QUIT_EVENT_H

#include "EventBase.h"

namespace Project {
namespace Event {

class QuitEvent : public EventBase {
public:
    QuitEvent() {}
    
    virtual EventType::type_t getType() const
        { return EventType::QUIT; }
};

}  // namespace Event
}  // namespace Project

#endif
