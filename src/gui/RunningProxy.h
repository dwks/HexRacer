#ifndef PROJECT_GUI__RUNNING_PROXY_H
#define PROJECT_GUI__RUNNING_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/PlayerProgressEvent.h"
#include "event/GameStageChanged.h"
#include "event/SwitchToScreen.h"

namespace Project {
namespace GUI {

class RunningProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void handleSwitchToScreen(Event::SwitchToScreen *event);
    void handleGameStageChanged(Event::GameStageChanged *event);
    void playerProgressHandler(Event::PlayerProgressEvent *event);
private:
    Widget::WidgetBase *running;
    std::string actualText;
public:
    RunningProxy(Widget::WidgetBase *running);
    
    virtual void visit(Widget::KeyEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
