#include "MainMenuProxy.h"

#include "widget/WidgetBase.h"
#include "widget/WidgetActivateEvent.h"

#include "event/QuitEvent.h"
#include "event/ObserverList.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void MainMenuProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "quit") {
        EMIT_EVENT(new Event::QuitEvent());
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
