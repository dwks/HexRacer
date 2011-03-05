#ifndef PROJECT_WIDGET__LIST_EVENT_PROXY_H
#define PROJECT_WIDGET__LIST_EVENT_PROXY_H

#include "widget/EventProxy.h"

namespace Project {
namespace Widget {

class ListWidget;

class ListEventProxy : public Widget::EventProxy {
private:
    ListWidget *widget;
public:
    ListEventProxy(ListWidget *widget) : widget(widget) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
    virtual void visit(FocusEvent *event);
    virtual void visit(WidgetActivateEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
