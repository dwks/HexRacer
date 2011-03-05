#ifndef PROJECT_WIDGET__SCROLLBAR_EVENT_PROXY_H
#define PROJECT_WIDGET__SCROLLBAR_EVENT_PROXY_H

#include "EventProxy.h"

namespace Project {
namespace Widget {

class ScrollbarWidget;

class ScrollbarEventProxy : public EventProxy {
private:
    ScrollbarWidget *widget;
public:
    ScrollbarEventProxy(ScrollbarWidget *widget) : widget(widget) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
