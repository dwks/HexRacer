#ifndef PROJECT_WIDGET__WIDGET_ACTIVATE_EVENT_H
#define PROJECT_WIDGET__WIDGET_ACTIVATE_EVENT_H

#include "WidgetEvent.h"
#include "WidgetPoint.h"

namespace Project {
namespace Widget {

class WidgetBase;

class WidgetActivateEvent : public WidgetEvent {
private:
    WidgetBase *widget;
    bool down;
public:
    WidgetActivateEvent(WidgetBase *widget, bool down)
        : widget(widget), down(down) {}
    
    WidgetBase *getWidget() { return widget; }
    bool getDown() const { return down; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_ACTIVATE; }
};

}  // namespace Widget
}  // namespace Project

#endif
