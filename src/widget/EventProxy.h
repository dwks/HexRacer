#ifndef PROJECT_WIDGET__EVENT_PROXY_H
#define PROJECT_WIDGET__EVENT_PROXY_H

#include "WidgetEvent.h"
#include "EventVisitor.h"

namespace Project {
namespace Widget {

class EventProxy : public EventVisitor {
public:
    virtual ~EventProxy() {}
    
    virtual void visit(MouseMoveEvent *event) {}
    virtual void visit(MouseButtonEvent *event) {}
    virtual void visit(WidgetActivateEvent *event) {}
};

}  // namespace Widget
}  // namespace Project

#endif
