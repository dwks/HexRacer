#ifndef PROJECT_GUI__PAUSE_MENU_PROXY_H
#define PROJECT_GUI__PAUSE_MENU_PROXY_H

#include "widget/EventProxy.h"

namespace Project {
namespace GUI {

class PauseMenuProxy : public Widget::EventProxy {
public:
    virtual void visit(Widget::WidgetActivateEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
