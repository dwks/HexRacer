#ifndef PROJECT_WIDGET__MOUSE_BUTTON_EVENT_H
#define PROJECT_WIDGET__MOUSE_BUTTON_EVENT_H

#include "WidgetEvent.h"
#include "WidgetPoint.h"

namespace Project {
namespace Widget {

class MouseButtonEvent : public WidgetEvent {
public:
    // these constants coincide with the SDL values
    enum button_t {
        BUTTON_LEFT = 1,
        BUTTON_MIDDLE = 2,
        BUTTON_RIGHT = 3
    };
private:
    WidgetPoint where;
    bool down;
    button_t button;
public:
    MouseButtonEvent(WidgetPoint where, bool down, button_t button)
        : where(where), down(down), button(button) {}
    
    WidgetPoint getWhere() const { return where; }
    bool getDown() const { return down; }
    button_t getButton() const { return button; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_MOUSE_BUTTON; }
};

}  // namespace Widget
}  // namespace Project

#endif
