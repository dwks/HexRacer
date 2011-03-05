#ifndef PROJECT_GUI__HOST_PROXY_H
#define PROJECT_GUI__HOST_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

namespace Project {
namespace GUI {

class HostProxy : public Widget::EventProxy {
private:
    Widget::WidgetBase *host;
public:
    HostProxy(Widget::WidgetBase *host) : host(host) {}
    
    virtual void visit(Widget::WidgetActivateEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
