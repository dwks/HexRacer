#include "LoadingProxy.h"

#include "event/EventSystem.h"
#include "event/ObserverRegistry.h"
#include "event/JoinGame.h"

#include "widget/ImageWidget.h"
#include "widget/TextWidget.h"
#include "widget/RepaintEvent.h"

#include "map/MapSettings.h"
#include "sdl/SpawnServer.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"
#include "misc/Sleeper.h"

namespace Project {
namespace GUI {

void LoadingProxy::initialize(Event::SwitchToScreen *event) {
    if(event->getScreen() != loading->getName()) return;
    
    Widget::ImageWidget *thumbnail
        = dynamic_cast<Widget::ImageWidget *>(loading->getChild("thumbnail"));
    
    if(Map::MapSettings::getInstance()->getGameType() == "connect") {
        thumbnail->setFilename("");
    }
    else {
        if(thumbnail) {
            thumbnail->setFilename(
                Map::MapSettings::getInstance()->getMapThumbnail());
        }
    }
    
    lastRepaint = NULL;
}

LoadingProxy::LoadingProxy(Widget::WidgetBase *loading,
    Widget::WidgetBase *host) : loading(loading), host(host) {
    
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
                
                Event::JoinGame joinGameEvent = Event::JoinGame(
                    GET_SETTING("network.host", "localhost"),
                    GET_SETTING("network.port", 1820));
                
                for(int i = 0; i < 30; i ++) {
                    LOG(NETWORK, "Connection attempt " << (i+1) << " of 30");
                    
                    Event::ObserverRegistry::getInstance().notifyObservers(
                        &joinGameEvent, false);
                    
                    if(joinGameEvent.getSuccess()) return;
                    
                    Misc::Sleeper::sleep(1000);
                }
                
                //EMIT_EVENT(new Event::SwitchToScreen(""));
                
                Widget::TextWidget *error = dynamic_cast<Widget::TextWidget *>(
                    host->getChild("error"));
                error->setText(Misc::StreamAsString()
                    << "Could not connect to server at "
                    << joinGameEvent.getHost()
                    << ":" << joinGameEvent.getPort());
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
