#include "HostProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"
#include "widget/EditWidget.h"

#include "event/SwitchToScreen.h"
#include "event/EventSystem.h"

#include "map/MapSettings.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void HostProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "map") {
        EMIT_EVENT(new Event::SwitchToScreen("selectmap"));
    }
    else if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen(""));
    }
    else if(name == "host") {
        std::string map = Map::MapSettings::getInstance()->getMapFile();
        if(map != "") {
            LOG(GUI, "Using map \"" << map << "\"");
            
            Map::MapSettings::getInstance()->setGameType("host");
            EMIT_EVENT(new Event::SwitchToScreen("loading"));
        }
        else {
            LOG2(GUI, WARNING, "No map selected!");
        }
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void HostProxy::visit(Widget::WidgetModifiedEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "hostport") {
        unsigned short hostport;
        std::istringstream stream(event->getWidget()->getData());
        if(!(stream >> hostport)) {
            LOG(GUI, "HostProxy: malformed host port \""
                << event->getWidget()->getData() << "\"");
            event->getWidget()->setData(
                GET_SETTING("network.serverport", "1820"));
            return;
        }
        
        Settings::SettingsManager::getInstance()->
            set("network.serverport", Misc::StreamAsString() << hostport);
        Settings::SettingsManager::getInstance()->
            set("network.port", Misc::StreamAsString() << hostport);
    }
    else if(name == "aicount") {
        int aicount;
        std::istringstream stream(event->getWidget()->getData());
        if(!(stream >> aicount) || aicount >= 100) {
            LOG(GUI, "HostProxy: malformed ai count \""
                << event->getWidget()->getData() << "\"");
            event->getWidget()->setData(
                GET_SETTING("server.aicount", "0"));
            return;
        }
        
        Settings::SettingsManager::getInstance()->
            set("server.aicount", Misc::StreamAsString() << aicount);
    }
    else if(name == "lapcount") {
        int lapcount;
        std::istringstream stream(event->getWidget()->getData());
        if(!(stream >> lapcount) || lapcount < 1 || lapcount >= 1000) {
            LOG(GUI, "HostProxy: malformed lap count \""
                << event->getWidget()->getData() << "\"");
            event->getWidget()->setData(
                GET_SETTING("game.race.laps", "3"));
            return;
        }
        
        Settings::SettingsManager::getInstance()->
            set("game.race.laps", Misc::StreamAsString() << lapcount);
    }
    else {
        LOG2(GUI, WARNING, "No action for modifying \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
