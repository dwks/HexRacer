#ifndef PROJECT_WIDGET__LIST_ITEM_EVENT_PROXY_H
#define PROJECT_WIDGET__LIST_ITEM_EVENT_PROXY_H

#include "EventProxy.h"

namespace Project {
namespace Widget {

class ListWidget;
class TextWidget;

class ListItemEventProxy : public EventProxy {
private:
    ListWidget *list;
    TextWidget *text;
public:
    ListItemEventProxy(ListWidget *list, TextWidget *text)
        : list(list), text(text) {}
    
    virtual void visit(MouseButtonEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
