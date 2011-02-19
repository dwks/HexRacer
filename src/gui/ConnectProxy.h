#ifndef PROJECT_GUI__CONNECT_PROXY_H
#define PROJECT_GUI__CONNECT_PROXY_H

#include "widget/EventProxy.h"

namespace Project {
namespace GUI {

class ConnectProxy : public Widget::EventProxy {
public:
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
