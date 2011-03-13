#ifndef PROJECT_GUI__ABOUT_PROXY_H
#define PROJECT_GUI__ABOUT_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

namespace Project {
namespace GUI {

class AboutProxy : public Widget::EventProxy {
private:
    Widget::WidgetBase *about;
public:
    AboutProxy(Widget::WidgetBase *about) : about(about) {}
    
    virtual void visit(Widget::WidgetActivateEvent *event);

};

}  // namespace GUI
}  // namespace Project

#endif
