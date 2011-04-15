#ifndef PROJECT_WIDGET__LIST_WIDGET_BOX_PROXY_H
#define PROJECT_WIDGET__LIST_WIDGET_BOX_PROXY_H

#include "EventProxy.h"

namespace Project {
namespace Widget {

class ListWidget;

class ListWidgetBoxProxy : public EventProxy {
private:
    ListWidget *list;
public:
    ListWidgetBoxProxy(ListWidget *list);
    
    virtual void visit(MouseButtonEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
