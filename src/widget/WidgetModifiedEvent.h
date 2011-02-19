#ifndef PROJECT_WIDGET__WIDGET_MODIFIED_EVENT_H
#define PROJECT_WIDGET__WIDGET_MODIFIED_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class EditWidget;

class WidgetModifiedEvent : public WidgetEvent {
private:
    EditWidget *widget;
public:
    WidgetModifiedEvent(EditWidget *widget) : widget(widget) {}
    
    EditWidget *getWidget() { return widget; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_MODIFIED; }
};

}  // namespace Widget
}  // namespace Project

#endif
