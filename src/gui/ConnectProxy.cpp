#include <sstream>
#include "ConnectProxy.h"

#include "widget/WidgetBase.h"
#include "widget/EditWidget.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"

#include "event/SwitchToScreen.h"
#include "event/JoinGame.h"
#include "event/EventSystem.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void ConnectProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen("main"));
    }
    else if(name == "connect") {
        EMIT_EVENT(new Event::JoinGame(
            GET_SETTING("network.host", "localhost").c_str(),
            GET_SETTING("network.port", 1820)));
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void ConnectProxy::visit(Widget::WidgetModifiedEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "host") {
        Settings::SettingsManager::getInstance()->set(
            "network.host", event->getWidget()->getData());
    }
    else if(name == "port") {
        std::istringstream stream(event->getWidget()->getData());
        unsigned short port;
        if(!(stream >> port)) {
            event->getWidget()->setData(GET_SETTING("network.port", "1820"));
            LOG(GUI, "Malformed port \""
                << event->getWidget()->getData() << "\"");
            return;
        }
        
        Settings::SettingsManager::getInstance()->set(
            "network.port", Misc::StreamAsString() << port);
    }
    else {
        LOG2(GUI, WARNING, "No action for modifying \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
