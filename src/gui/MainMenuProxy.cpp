#include "MainMenuProxy.h"

#include "widget/WidgetBase.h"
#include "widget/WidgetActivateEvent.h"

#include "event/QuitEvent.h"
#include "event/JoinGame.h"
#include "event/ObserverList.h"

#include "log/Logger.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace GUI {

void MainMenuProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "quit") {
        EMIT_EVENT(new Event::QuitEvent());
    }
    else if(name == "join") {
        EMIT_EVENT(new Event::JoinGame(
            GET_SETTING("network.host", "localhost").c_str(),
            GET_SETTING("network.port", 1820)));
    }
    else if(name == "single") {
        EMIT_EVENT(new Event::JoinGame());
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
