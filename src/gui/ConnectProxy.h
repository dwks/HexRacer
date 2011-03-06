#ifndef PROJECT_GUI__CONNECT_PROXY_H
#define PROJECT_GUI__CONNECT_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/SwitchToScreen.h"

namespace Project {
namespace GUI {

class ConnectProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void initialize(Event::SwitchToScreen *event);
private:
    Widget::WidgetBase *connect;
public:
    ConnectProxy(Widget::WidgetBase *connect);
    
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
private:
    void clearError();
};

}  // namespace GUI
}  // namespace Project

#endif
