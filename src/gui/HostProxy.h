#ifndef PROJECT_GUI__HOST_PROXY_H
#define PROJECT_GUI__HOST_PROXY_H

#include <string>

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

namespace Project {
namespace GUI {

class HostProxy : public Widget::EventProxy {
private:
    Widget::WidgetBase *host;
    static std::string map;  // augh, smart pointers please . . . .
public:
    HostProxy(Widget::WidgetBase *host) : host(host) {}
    
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
