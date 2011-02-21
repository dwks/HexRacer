#include "PauseMenuProxy.h"

#include "widget/WidgetBase.h"
#include "widget/WidgetActivateEvent.h"

#include "event/PauseGame.h"
#include "event/QuitEvent.h"
#include "event/EventSystem.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void PauseMenuProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "resume") {
        LOG(GUI, "Game unpaused via button");
        EMIT_EVENT(new Event::PauseGame(false));
    }
    else if(name == "quit") {
        EMIT_EVENT(new Event::QuitEvent());
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
