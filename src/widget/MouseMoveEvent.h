#ifndef PROJECT_WIDGET__MOUSE_MOVE_EVENT_H
#define PROJECT_WIDGET__MOUSE_MOVE_EVENT_H

#include "WidgetEvent.h"
#include "WidgetPoint.h"

namespace Project {
namespace Widget {

class MouseMoveEvent : public WidgetEvent {
private:
    WidgetPoint where;
public:
    MouseMoveEvent(WidgetPoint where) : where(where) {}
    
    WidgetPoint getWhere() const { return where; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_MOUSE_MOVE; }
};

}  // namespace Widget
}  // namespace Project

#endif
