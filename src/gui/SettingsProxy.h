#ifndef PROJECT_GUI__SETTINGS_PROXY_H
#define PROJECT_GUI__SETTINGS_PROXY_H

#include "widget/EventProxy.h"

namespace Project {
namespace GUI {

class SettingsProxy : public Widget::EventProxy {
public:
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
