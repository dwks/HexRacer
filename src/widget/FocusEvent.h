#ifndef PROJECT_WIDGET__FOCUS_EVENT_H
#define PROJECT_WIDGET__FOCUS_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class FocusEvent : public WidgetEvent {
public:
    enum focus_t {
        FOCUS_KEY,
        FOCUS_MOTION,
        FOCUS_CLICK
    };
private:
    focus_t focus;
    bool gained;
public:
    FocusEvent(focus_t focus, bool gained) : focus(focus), gained(gained) {}
    
    focus_t getFocus() const { return focus; }
    bool wasGained() const { return gained; }
    bool wasLost() const { return !gained; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_FOCUS; }
};

}  // namespace Widget
}  // namespace Project

#endif
