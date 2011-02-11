#ifndef PROJECT_GUI__RUNNING_PROXY_H
#define PROJECT_GUI__RUNNING_PROXY_H

#include "widget/EventProxy.h"

namespace Project {
namespace GUI {

class RunningProxy : public Widget::EventProxy {
public:
    virtual void visit(Widget::WidgetActivateEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
