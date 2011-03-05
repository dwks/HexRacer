#ifndef PROJECT_WIDGET__CHECK_EVENT_PROXY_H
#define PROJECT_WIDGET__CHECK_EVENT_PROXY_H

#include "EventProxy.h"

namespace Project {
namespace Widget {

class CheckWidget;

class CheckEventProxy : public EventProxy {
private:
    CheckWidget *widget;
public:
    CheckEventProxy(CheckWidget *widget) : widget(widget) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
    virtual void visit(FocusEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
