#include "LobbyProxy.h"

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

void LobbyProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "map") {
        EMIT_EVENT(new Event::SwitchToScreen("selectmap"));
    }
    else if(name == "debug") {
        EMIT_EVENT(new Event::SwitchToScreen("lobby"));
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

}  // namespace GUI
}  // namespace Project
