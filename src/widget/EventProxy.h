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
    virtual void visit(KeyEvent *event) {}
    virtual void visit(WidgetActivateEvent *event) {}
    virtual void visit(FocusEvent *event) {}
    virtual void visit(WidgetModifiedEvent *event) {}
    virtual void visit(WidgetSelectedEvent *event) {}
    virtual void visit(RepaintEvent *event) {}
	virtual void visit(MenuMoveEvent *event) {}
	virtual void visit(MenuSelectedEvent *event) {}
};

}  // namespace Widget
}  // namespace Project

#endif
