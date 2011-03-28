#include "AboutProxy.h"

#include "widget/WidgetActivateEvent.h"

#include "event/EventSystem.h"
#include "event/SwitchToScreen.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void AboutProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "back") {
        EMIT_EVENT(new Event::SwitchToScreen(""));
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
