#ifndef PROJECT_WIDGET__COMPOSITE_EVENT_PROXY_H
#define PROJECT_WIDGET__COMPOSITE_EVENT_PROXY_H

#include "EventProxy.h"
#include "CompositeWidget.h"

namespace Project {
namespace Widget {

class CompositeEventProxy : public EventProxy {
private:
    CompositeWidget *widget;
public:
    CompositeEventProxy(CompositeWidget *widget) : widget(widget) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
    virtual void visit(KeyEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
