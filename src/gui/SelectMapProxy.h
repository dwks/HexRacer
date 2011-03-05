#ifndef PROJECT_GUI__SELECT_MAP_PROXY_H
#define PROJECT_GUI__SELECT_MAP_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/SwitchToScreen.h"

namespace Project {
namespace GUI {

class SelectMapProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void initialize(Event::SwitchToScreen *event);
private:
    Widget::WidgetBase *selectmap;
public:
    SelectMapProxy(Widget::WidgetBase *selectmap);
    
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
private:
    void useMap(const std::string &file);
};

}  // namespace GUI
}  // namespace Project

#endif
