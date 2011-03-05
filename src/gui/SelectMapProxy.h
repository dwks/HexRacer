#ifndef PROJECT_GUI__SELECT_MAP_PROXY_H
#define PROJECT_GUI__SELECT_MAP_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

namespace Project {
namespace GUI {

class SelectMapProxy : public Widget::EventProxy {
private:
    Widget::WidgetBase *selectmap;
public:
    SelectMapProxy(Widget::WidgetBase *selectmap) : selectmap(selectmap) {}
    
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
