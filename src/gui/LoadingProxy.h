#ifndef PROJECT_GUI__LOADING_PROXY_H
#define PROJECT_GUI__LOADING_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/SwitchToScreen.h"

namespace Project {
namespace GUI {

class LoadingProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void initialize(Event::SwitchToScreen *event);
private:
    Widget::WidgetBase *loading;
    Widget::WidgetBase *lastRepaint;
public:
    LoadingProxy(Widget::WidgetBase *loading);
    
    virtual void visit(Widget::RepaintEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
