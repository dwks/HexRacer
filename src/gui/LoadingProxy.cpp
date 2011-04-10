#include "LoadingProxy.h"

#include "event/EventSystem.h"
#include "event/ObserverRegistry.h"
#include "event/JoinGame.h"
#include "event/StartingGame.h"
#include "event/SetCheckingNetwork.h"
#include "event/SetupClientSettings.h"

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

void LoadingProxy::gameStateChanged(Event::GameStageChanged *event) {
    if(event->getStage() == World::WorldSetup::START_LOADING) {
#if 0
        EMIT_EVENT(new Event::StartingGame(
            Event::StartingGame::LOADING_MAP));
#else
        EMIT_EVENT(new Event::SetCheckingNetwork(false));
        
        lastRepaint = NULL;
        Map::MapSettings::getInstance()->setGameType("loadingworld");
        EMIT_EVENT(new Event::SwitchToScreen("loading"));
#endif
    }
}

LoadingProxy::LoadingProxy(Widget::WidgetBase *loading,
    Widget::WidgetBase *host) : loading(loading), host(host) {
    
    lastRepaint = NULL;
    
    METHOD_OBSERVER(&LoadingProxy::initialize);
    METHOD_OBSERVER(&LoadingProxy::gameStateChanged);
}

void LoadingProxy::visit(Widget::RepaintEvent *event) {
    if(lastRepaint != event->getWidget()) {
        lastRepaint = event->getWidget();
        
        if(event->getWidget() == loading) {
            std::string type = Map::MapSettings::getInstance()->getGameType();
            
            if(type == "connect") {
                Event::JoinGame *join = new Event::JoinGame(
                    GET_SETTING("network.host", "localhost"),
                    GET_SETTING("network.port", 1820));
                Event::ObserverRegistry::getInstance()
                    .notifyObservers(join, false);
                
                if(join->getSuccess()) {
                    EMIT_EVENT(new Event::SwitchToScreen("lobby"));
                }
                else {
                    EMIT_EVENT(new Event::SwitchToScreen(""));
                }
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
                    
                    if(joinGameEvent.getSuccess()) {
                        EMIT_EVENT(new Event::SwitchToScreen("lobby"));
                        return;
                    }
                    
                    Misc::Sleeper::sleep(1000);
                }
                
                //EMIT_EVENT(new Event::SwitchToScreen(""));
                
                Widget::TextWidget *error = dynamic_cast<Widget::TextWidget *>(
                    host->getChild("error"));
                error->setText(Misc::StreamAsString()
                    << "Could not connect to server at "
                    << joinGameEvent.getHost()
                    << ":" << joinGameEvent.getPort());
                
                EMIT_EVENT(new Event::SwitchToScreen(""));
            }
            else if(type == "loadingworld") {
                EMIT_EVENT(new Event::SetCheckingNetwork(true));
                
                EMIT_EVENT(new Event::StartingGame(
                    Event::StartingGame::LOADING_MAP));
                
                announceFinishedLoading();
            }
            else if(type == "singleplayer") {
                EMIT_EVENT(new Event::JoinGame());
                EMIT_EVENT(new Event::StartingGame(
                    Event::StartingGame::LOADING_MAP));
                
                EMIT_EVENT(new Event::GameStageChanged(
                    World::WorldSetup::START_COUNTDOWN));
            }
            else if(type == "waiting") {
                // do nothing while waiting
            }
            else {
                LOG(GUI, "Unknown game type \"" << type << "\"");
                EMIT_EVENT(new Event::SwitchToScreen(""));
            }
        }
    }
}

void LoadingProxy::announceFinishedLoading() {
    World::WorldSetup *worldSetup = World::WorldSetup::getInstance();
    
    int id = worldSetup->getClientID();
    World::WorldSetup::ClientSettings *settings
        = worldSetup->getClientSettings(id);
    if(settings) {
        settings->setFullyLoaded(true);
        
        EMIT_EVENT(new Event::SetupClientSettings(*settings));
    }
    
    Widget::TextWidget *text = dynamic_cast<Widget::TextWidget *>(
        loading->getChild("loading"));
    if(text) {
        text->setText("Waiting ...");
    }
    
    Map::MapSettings::getInstance()->setGameType("waiting");
    EMIT_EVENT(new Event::SwitchToScreen("loading"));
}

}  // namespace GUI
}  // namespace Project
