#ifndef PROJECT_WIDGET__WIDGET_EVENT_H
#define PROJECT_WIDGET__WIDGET_EVENT_H

#include "EventVisitor.h"

namespace Project {
namespace Widget {

/** Abstract base class for all widget event types.
*/
class WidgetEvent {
public:
    enum event_type_t {
        EVENT_MOUSE_MOVE,
        EVENT_MOUSE_BUTTON,
        EVENT_ACTIVATE
    };
public:
    WidgetEvent() {}
    virtual ~WidgetEvent() {}
    
    virtual void accept(EventVisitor &visitor) = 0;
    virtual event_type_t getType() const = 0;
};

}  // namespace Widget
}  // namespace Project

#endif
