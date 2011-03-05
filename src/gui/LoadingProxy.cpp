#include "LoadingProxy.h"
#include "event/EventSystem.h"
#include "event/JoinGame.h"

#include "widget/ImageWidget.h"
#include "widget/RepaintEvent.h"

#include "map/MapSettings.h"
#include "sdl/SpawnServer.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void LoadingProxy::initialize(Event::SwitchToScreen *event) {
    if(event->getScreen() != loading->getName()) return;
    
    Widget::ImageWidget *thumbnail
        = dynamic_cast<Widget::ImageWidget *>(loading->getChild("thumbnail"));
    
    if(thumbnail) {
        thumbnail->setFilename(
            Map::MapSettings::getInstance()->getMapThumbnail());
    }
    
    lastRepaint = NULL;
}

LoadingProxy::LoadingProxy(Widget::WidgetBase *loading) : loading(loading) {
    lastRepaint = NULL;
    
    METHOD_OBSERVER(&LoadingProxy::initialize);
}

void LoadingProxy::visit(Widget::RepaintEvent *event) {
    if(lastRepaint != event->getWidget()) {
        lastRepaint = event->getWidget();
        
        if(event->getWidget() == loading) {
            std::string type = Map::MapSettings::getInstance()->getGameType();
            
            if(type == "connect") {
                EMIT_EVENT(new Event::JoinGame(
                    GET_SETTING("network.host", "localhost"),
                    GET_SETTING("network.port", 1820)));
            }
            else if(type == "host") {
                SDL::SpawnServer spawner;
                spawner.spawn();
                
                EMIT_EVENT(new Event::JoinGame(
                    GET_SETTING("network.host", "localhost"),
                    GET_SETTING("network.port", 1820)));
            }
            else if(type == "singleplayer") {
                EMIT_EVENT(new Event::JoinGame());
            }
            else {
                LOG(GUI, "Unknown game type \"" << type << "\"");
                EMIT_EVENT(new Event::SwitchToScreen(""));
            }
        }
    }
}

}  // namespace GUI
}  // namespace Project
