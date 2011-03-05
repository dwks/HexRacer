#include "SinglePlayerProxy.h"

#include "widget/WidgetActivateEvent.h"

#include "event/SwitchToScreen.h"
#include "event/EventSystem.h"

#include "map/MapSettings.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void SinglePlayerProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "map") {
        EMIT_EVENT(new Event::SwitchToScreen("selectmap"));
    }
    else if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen(""));
    }
    else if(name == "start") {
        std::string map = Map::MapSettings::getInstance()->getMapFile();
        if(map != "") {
            LOG(GUI, "Using map \"" << map << "\"");
            
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
