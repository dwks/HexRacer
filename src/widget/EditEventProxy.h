#ifndef PROJECT_WIDGET__EDIT_EVENT_PROXY_H
#define PROJECT_WIDGET__EDIT_EVENT_PROXY_H

#include "EventProxy.h"
#include "EditWidget.h"

namespace Project {
namespace Widget {

class EditEventProxy : public EventProxy {
private:
    EditWidget *widget;
public:
    EditEventProxy(EditWidget *widget) : widget(widget) {}
    
    virtual void visit(MouseMoveEvent *event);
    virtual void visit(MouseButtonEvent *event);
    virtual void visit(KeyEvent *event);
    virtual void visit(WidgetActivateEvent *event);
    virtual void visit(FocusEvent *event);
private:
    void saveText();
};

}  // namespace Widget
}  // namespace Project

#endif
