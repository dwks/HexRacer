#include <sstream>

#include "SettingsProxy.h"

#include "widget/WidgetBase.h"
#include "widget/EditWidget.h"
#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"

#include "event/SwitchToScreen.h"
#include "event/ObserverList.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void SettingsProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "accept") {
        EMIT_EVENT(new Event::SwitchToScreen("main"));
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void SettingsProxy::visit(Widget::WidgetModifiedEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "screenmode") {
        int width, height;
        char x;
        std::istringstream stream(event->getWidget()->getData());
        if(!(stream >> width >> x >> height) || x != 'x') {
            LOG(GUI, "SettingsProxy: malformed screen mode \""
                << event->getWidget()->getData() << "\"");
            event->getWidget()->setData(Misc::StreamAsString()
                << GET_SETTING("display.width", 0) << 'x'
                << GET_SETTING("display.height", 0));
            return;
        }
        
        Settings::SettingsManager::getInstance()->set(
            "display.width", Misc::StreamAsString() << width);
        Settings::SettingsManager::getInstance()->set(
            "display.height", Misc::StreamAsString() << height);
        int bpp = GET_SETTING("display.bpp", 0);
        
        int sdl_init_flags = SDL_HWSURFACE | SDL_OPENGL | SDL_RESIZABLE;
        if(GET_SETTING("display.fullscreen", 0)) {
            sdl_init_flags |= SDL_FULLSCREEN;
        }
        
        SDL_SetVideoMode(width, height, bpp, sdl_init_flags);
    }
    else {
        LOG2(GUI, WARNING, "No action for modifying \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
