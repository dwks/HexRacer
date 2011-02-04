#ifndef PROJECT_WIDGET__WIDGET_FOCUS_EVENT_H
#define PROJECT_WIDGET__WIDGET_FOCUS_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class WidgetFocusEvent : public WidgetEvent {
public:
    enum focus_t {
        KEYBOARD_FOCUS,
        MOUSE_FOCUS
    };
private:
    unsigned focus;
public:
    WidgetFocusEvent(unsigned focus) : focus(focus) {}
    
    unsigned getFocus() const { return focus; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_FOCUS; }
};

}  // namespace Widget
}  // namespace Project

#endif
