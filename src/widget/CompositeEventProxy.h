#ifndef PROJECT_WIDGET__COMPOSITE_EVENT_PROXY_H
#define PROJECT_WIDGET__COMPOSITE_EVENT_PROXY_H

#include "EventProxy.h"
#include "CompositeWidget.h"

namespace Project {
namespace Widget {

class CompositeEventProxy : public EventProxy {
private:
    CompositeWidget *widget;
    WidgetBase *proxy;
public:
    CompositeEventProxy(CompositeWidget *widget) : widget(widget), proxy(0) {}
    
    virtual void visit(WidgetFocusEvent *event);
    virtual void visit(WidgetActivateEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
