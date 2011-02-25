#ifndef PROJECT_WIDGET__SCROLLBAR_EVENT_PROXY_H
#define PROJECT_WIDGET__SCROLLBAR_EVENT_PROXY_H

#include "widget/EventProxy.h"

namespace Project {
namespace Widget {

class ScrollbarWidget;

class ScrollbarEventProxy : public Widget::EventProxy {
private:
    ScrollbarWidget *widget;
public:
    ScrollbarEventProxy(ScrollbarWidget *widget) : widget(widget) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
    virtual void visit(FocusEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
