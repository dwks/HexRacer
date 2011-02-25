#include "HostProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"
#include "widget/WidgetSelectedEvent.h"

#include "widget/TextWidget.h"
#include "widget/ImageWidget.h"

#include "event/SwitchToScreen.h"
#include "event/JoinGame.h"
#include "event/EventSystem.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

std::string HostProxy::map;

void HostProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen("main"));
    }
    else if(name == "host") {
        //LOG2(GUI, WARNING, "Hosting not yet implemented");
        
        if(map != "") {
            LOG(GUI, "Using map \"" << map << "\"");
            
            Settings::SettingsManager::getInstance()->set("map", map);
            EMIT_EVENT(new Event::JoinGame());
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
    
    this->map = GET_SETTING(root, "");
}

}  // namespace GUI
}  // namespace Project
