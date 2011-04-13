#include "SelectMapProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"
#include "widget/WidgetSelectedEvent.h"

#include "widget/TextWidget.h"
#include "widget/ImageWidget.h"
#include "widget/ListWidget.h"

#include "event/SwitchToScreen.h"
#include "event/EventSystem.h"

#include "map/MapSettings.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void SelectMapProxy::initialize(Event::SwitchToScreen *event) {
    if(event->getScreen() == "selectmap") {
        Map::MapSettings *mapSettings = Map::MapSettings::getInstance();
        
        useMap(mapSettings->getMapFile());
    }
}

SelectMapProxy::SelectMapProxy(Widget::WidgetBase *selectmap)
    : selectmap(selectmap) {
    
    METHOD_OBSERVER(&SelectMapProxy::initialize);
}

void SelectMapProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "selectmap") {
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
    
    LOG(GUI, "selected map \"" << title << "\"");
    useMap(file);
}

void SelectMapProxy::useMap(const std::string &file) {
    Map::MapSettings *mapSettings = Map::MapSettings::getInstance();
    
    std::string title = mapSettings->getMap(file).getTitle();
    std::string thumbnail = mapSettings->getMap(file).getThumbnail();
    
    dynamic_cast<Widget::TextWidget *>(selectmap->getChild("title"))
        ->setText(title);
    dynamic_cast<Widget::ImageWidget *>(selectmap->getChild("thumbnail"))
        ->setFilename(thumbnail);
    dynamic_cast<Widget::ListWidget *>(selectmap->getChild("maplist"))
        ->setLastSelected(title);
    
    Map::MapSettings::getInstance()->setMapFile(file);
}

}  // namespace GUI
}  // namespace Project
