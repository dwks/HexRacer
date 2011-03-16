#ifndef PROJECT_GUI__SETTINGS_PROXY_H
#define PROJECT_GUI__SETTINGS_PROXY_H

#include "widget/WidgetBase.h"
#include "widget/EventProxy.h"

namespace Project {
namespace GUI {

class SettingsProxy : public Widget::EventProxy {
private:
    Widget::WidgetBase *settings;
    bool checkingJoystick;
public:
    SettingsProxy(Widget::WidgetBase *settings)
        : settings(settings), checkingJoystick(false) {}
    
    virtual void visit(Widget::RepaintEvent *event);
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
