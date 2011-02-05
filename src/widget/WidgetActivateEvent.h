#ifndef PROJECT_WIDGET__WIDGET_ACTIVATE_EVENT_H
#define PROJECT_WIDGET__WIDGET_ACTIVATE_EVENT_H

#include "WidgetEvent.h"
#include "WidgetPoint.h"

namespace Project {
namespace Widget {

class WidgetActivateEvent : public WidgetEvent {
private:
    bool down;
public:
    WidgetActivateEvent(bool down) : down(down) {}
    
    bool getDown() const { return down; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_ACTIVATE; }
};

}  // namespace Widget
}  // namespace Project

#endif
