#include "HostProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"

#include "event/SwitchToScreen.h"
#include "event/EventSystem.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void HostProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen("main"));
    }
    else if(name == "host") {
        LOG2(GUI, WARNING, "Hosting not yet implemented");
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void HostProxy::visit(Widget::WidgetModifiedEvent *event) {
    LOG2(GUI, WARNING, "HostProxy doesn't handle WidgetModifiedEvent");
}

}  // namespace GUI
}  // namespace Project
