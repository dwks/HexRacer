#include "RunningProxy.h"

#include "widget/WidgetBase.h"
#include "widget/WidgetActivateEvent.h"
#include "event/PauseGame.h"

#include "event/EventSystem.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void RunningProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "menu") {
        LOG(GUI, "Game paused via menu button");
        EMIT_EVENT(new Event::PauseGame(true));
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
