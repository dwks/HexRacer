#ifndef PROJECT_WIDGET__WIDGET_ACTIVATE_EVENT_H
#define PROJECT_WIDGET__WIDGET_ACTIVATE_EVENT_H

#include "WidgetEvent.h"

namespace Project {
namespace Widget {

class WidgetActivateEvent : public WidgetEvent {
public:
    enum activate_t {
        BEGIN,   //!< clicking
        CANCEL,  //!< stopped clicking
        EXECUTE  //!< released mouse inside widget
    };
private:
    unsigned activate;
public:
    WidgetActivateEvent(unsigned activate) {}
    
    unsigned getActivate() const { return activate; }
    
    virtual void accept(EventVisitor &visitor) { visitor.visit(this); }
    virtual event_type_t getType() const { return EVENT_ACTIVATE; }
};

}  // namespace Widget
}  // namespace Project

#endif
