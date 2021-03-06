#ifndef PROJECT_WIDGET__BUTTON_EVENT_PROXY_H
#define PROJECT_WIDGET__BUTTON_EVENT_PROXY_H

#include "EventProxy.h"
#include "ButtonWidget.h"

namespace Project {
namespace Widget {

class ButtonEventProxy : public EventProxy {
private:
    ButtonWidget *widget;
public:
    ButtonEventProxy(ButtonWidget *widget) : widget(widget) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
    virtual void visit(FocusEvent *event);
};

}  // namespace Widget
}  // namespace Project

#endif
