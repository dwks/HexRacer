#include "SettingsProxy.h"

#include "widget/WidgetBase.h"
#include "widget/WidgetActivateEvent.h"

#include "event/SwitchToScreen.h"
#include "event/ObserverList.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void SettingsProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "accept") {
        EMIT_EVENT(new Event::SwitchToScreen("main"));
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
