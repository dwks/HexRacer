#include <sstream>
#include "ConnectProxy.h"

#include "widget/WidgetBase.h"
#include "widget/EditWidget.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"

#include "event/EventSystem.h"

#include "map/MapSettings.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void ConnectProxy::initialize(Event::SwitchToScreen *event) {
    if(event->getScreen() != "connect") return;
    
    Widget::EditWidget *port
        = dynamic_cast<Widget::EditWidget *>(connect->getChild("port"));
    if(port) {
        port->setData(GET_SETTING("network.port", "1820"));
    }
}

ConnectProxy::ConnectProxy(Widget::WidgetBase *connect) : connect(connect) {
    METHOD_OBSERVER(&ConnectProxy::initialize);
}

void ConnectProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        clearError();
        EMIT_EVENT(new Event::SwitchToScreen("main"));
    }
    else if(name == "connect") {
        clearError();
        
        Map::MapSettings::getInstance()->setGameType("connect");
        EMIT_EVENT(new Event::SwitchToScreen("loading"));
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

void ConnectProxy::clearError() {
    Widget::TextWidget *error
        = dynamic_cast<Widget::TextWidget *>(connect->getChild("error"));
    if(error) {
        error->setText("");
    }
}

}  // namespace GUI
}  // namespace Project
