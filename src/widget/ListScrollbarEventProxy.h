#ifndef PROJECT_WIDGET__LIST_SCROLLBAR_EVENT_PROXY_H
#define PROJECT_WIDGET__LIST_SCROLLBAR_EVENT_PROXY_H

#include "widget/EventProxy.h"

namespace Project {
namespace Widget {

class ListWidget;

class ListScrollbarEventProxy : public Widget::EventProxy {
private:
    ListWidget *list;
public:
    ListScrollbarEventProxy(ListWidget *list) : list(list) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(WidgetActivateEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
