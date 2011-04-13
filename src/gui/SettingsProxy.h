#ifndef PROJECT_GUI__SETTINGS_PROXY_H
#define PROJECT_GUI__SETTINGS_PROXY_H

#include "widget/WidgetBase.h"
#include "widget/EventProxy.h"

#include "event/SwitchToScreen.h"
#include "event/Enabler.h"

namespace Project {
namespace GUI {

class SettingsProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void handleSwitchToScreen(Event::SwitchToScreen *event);
private:
    Widget::WidgetBase *settings;
    bool checkingJoystick;
public:
    SettingsProxy(Widget::WidgetBase *settings);
    
    virtual void visit(Widget::RepaintEvent *event);
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
private:
    const char *qualityString();
    void setCheckingForJoystick(bool yes);
};

}  // namespace GUI
}  // namespace Project

#endif
