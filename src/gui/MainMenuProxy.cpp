#include "MainMenuProxy.h"

#include "widget/WidgetBase.h"
#include "widget/WidgetActivateEvent.h"

#include "event/QuitEvent.h"
#include "event/JoinGame.h"
#include "event/SwitchToScreen.h"
#include "event/EventSystem.h"

#include "log/Logger.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace GUI {

void MainMenuProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "host") {
        EMIT_EVENT(new Event::SwitchToScreen("host"));
    }
    else if(name == "join") {
        EMIT_EVENT(new Event::SwitchToScreen("connect"));
    }
    else if(name == "single") {
        EMIT_EVENT(new Event::JoinGame());
    }
    else if(name == "settings") {
        EMIT_EVENT(new Event::SwitchToScreen("settings"));
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
