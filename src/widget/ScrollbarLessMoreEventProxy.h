#ifndef PROJECT_WIDGET__SCROLLBAR_LESS_MORE_EVENT_PROXY_H
#define PROJECT_WIDGET__SCROLLBAR_LESS_MORE_EVENT_PROXY_H

#include "EventProxy.h"

namespace Project {
namespace Widget {

class ScrollbarWidget;

class ScrollbarLessMoreEventProxy : public EventProxy {
private:
    ScrollbarWidget *widget;
public:
    ScrollbarLessMoreEventProxy(ScrollbarWidget *widget) : widget(widget) {}
    
    virtual void visit(WidgetActivateEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
