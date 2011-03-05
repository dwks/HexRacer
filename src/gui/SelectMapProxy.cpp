#include "SelectMapProxy.h"

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

void SelectMapProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen(""));
    }
    else if(name == "selectmap") {
        std::string map = Map::MapSettings::getInstance()->getMapFile();
        if(map != "") {
            LOG(GUI, "Using map \"" << map << "\"");
            
            EMIT_EVENT(new Event::SwitchToScreen(""));
        }
        else {
            LOG2(GUI, WARNING, "No map selected!");
        }
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void SelectMapProxy::visit(Widget::WidgetModifiedEvent *event) {
    LOG2(GUI, WARNING, "SelectMapProxy doesn't handle WidgetModifiedEvent");
}

void SelectMapProxy::visit(Widget::WidgetSelectedEvent *event) {
    std::string file = event->getSelected()->getName();
    std::string title = event->getSelected()->getData();
    std::string thumbnail = Map::MapSettings::getInstance()
        ->getMap(file).getThumbnail();
    
    LOG(GUI, "selected map \"" << title << "\"");
    
    dynamic_cast<Widget::TextWidget *>(selectmap->getChild("title"))
        ->setText(title);
    dynamic_cast<Widget::ImageWidget *>(selectmap->getChild("thumbnail"))
        ->setFilename(thumbnail);
    
    Map::MapSettings::getInstance()->setMapFile(file);
}

}  // namespace GUI
}  // namespace Project
