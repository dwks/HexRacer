#ifndef PROJECT_GUI__RUNNING_PROXY_H
#define PROJECT_GUI__RUNNING_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/PlayerProgressEvent.h"

namespace Project {
namespace GUI {

class RunningProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void playerProgressHandler(Event::PlayerProgressEvent *event);
private:
    Widget::WidgetBase *running;
public:
    RunningProxy(Widget::WidgetBase *running);
    
    virtual void visit(Widget::KeyEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
