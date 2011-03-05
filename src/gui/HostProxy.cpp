#include "HostProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"
#include "widget/WidgetSelectedEvent.h"

#include "widget/TextWidget.h"
#include "widget/ImageWidget.h"

#include "event/SwitchToScreen.h"
#include "event/EventSystem.h"

#include "map/MapSettings.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void HostProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen("main"));
    }
    else if(name == "host") {
        //LOG2(GUI, WARNING, "Hosting not yet implemented");
        
        std::string map = Map::MapSettings::getInstance()->getMapFile();
        if(map != "") {
            LOG(GUI, "Using map \"" << map << "\"");
            
            //EMIT_EVENT(new Event::JoinGame());
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
    LOG2(GUI, WARNING, "HostProxy doesn't handle WidgetModifiedEvent");
}

void HostProxy::visit(Widget::WidgetSelectedEvent *event) {
    std::string title = event->getSelected()->getData();
    LOG(GUI, "selected map \"" << title << "\"");
    
    std::string root = event->getSelected()->getName();
    std::string thumbnail = GET_SETTING(root + ".image", "");
    
    dynamic_cast<Widget::TextWidget *>(host->getChild("title"))
        ->setText(title);
    
    dynamic_cast<Widget::ImageWidget *>(host->getChild("thumbnail"))
        ->setFilename(thumbnail);
    
    Map::MapSettings::getInstance()->setMapFile(GET_SETTING(root, ""));
    Map::MapSettings::getInstance()->setMapThumbnail(thumbnail);
    Map::MapSettings::getInstance()->setMapTitle(title);
}

}  // namespace GUI
}  // namespace Project
