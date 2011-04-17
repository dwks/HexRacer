#include <sstream>

#include "LobbyProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"
#include "widget/WidgetSelectedEvent.h"
#include "widget/EditWidget.h"
#include "widget/ListWidget.h"

#include "event/EventSystem.h"
#include "event/SwitchToScreen.h"
#include "event/SetupPlayerSettings.h"
#include "event/SetupClientSettings.h"
#include "event/GeneralWorldSetupEvent.h"

#include "map/MapSettings.h"
#include "map/Teams.h"
#include "object/Player.h"

#include "settings/SettingsManager.h"
#include "misc/StreamAsString.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void LobbyProxy::handleSetupChat(Event::SetupChat *event) {
    // wait until the message comes back from the server
    if(event->getJustSpoken()) return;
    
    static int chatCount = 0;
    
    int id = event->getClient();
    std::string name = event->getName();
    std::string message = event->getMessage();
    
    Widget::TextWidget *item;
    
    if(event->getName() == "") {
        item = new Widget::TextWidget(
            Misc::StreamAsString() << "chat" << ++chatCount,
            message,
            Widget::NormalTextLayout::ALIGN_LEFT,
            Widget::WidgetRect(0.0, 0.0, 0.8, 0.03));
    }
    else {
        item = new Widget::TextWidget(
            Misc::StreamAsString() << "chat" << ++chatCount,
            Misc::StreamAsString() << name << ": " << message,
            Widget::NormalTextLayout::ALIGN_LEFT,
            Widget::WidgetRect(0.0, 0.0, 0.8, 0.03),
            Map::Teams::teamColor(worldSetup->getPlayerSettings(id)->getColor()));
    }
    
    Widget::ListWidget *chat = dynamic_cast<Widget::ListWidget *>(
        this->lobby->getChild("chatlist"));
    if(chat) {
        chat->addChild(item);
    }
    else {
        LOG2(GUI, ERROR, "Can't find chat widget to add message to");
    }
}

void LobbyProxy::handleSetupPlayerSettings(Event::SetupPlayerSettings *event) {
    worldSetup->replacePlayerSettings(event->getPlayerSettings());
}

void LobbyProxy::handleReplaceWorldSetup(Event::ReplaceWorldSetup *event) {
    LOG(NETWORK, "ReplaceWorldSetup received");
    
    int id = worldSetup->getClientID();
    *worldSetup = *event->getWorldSetup();
    worldSetup->setClientID(id);
    
    World::WorldSetup::PlayerSettings *settings
        = worldSetup->getPlayerSettings(id);
    if(settings) {
        dynamic_cast<Widget::EditWidget *>(lobby->getChild("playername"))
            ->setData(settings->getName());
    }
    
    Widget::ListWidget *playerList = dynamic_cast<Widget::ListWidget *>(
        lobby->getChild("playerlist"));
    
    if(playerList) {
        playerList->removeAllChildren();
        
        std::vector<int> ids;
        worldSetup->getAllPlayerIDs(ids);
        for(std::vector<int>::iterator i = ids.begin(); i != ids.end(); ++ i) {
            int id = *i;
            
            World::WorldSetup::PlayerSettings *settings
                = worldSetup->getPlayerSettings(id);
            
            std::string name = settings->getName();
            
            static int playerItemCount = 0;
            Widget::TextWidget *item = new Widget::TextWidget(
                Misc::StreamAsString() << "playeritem" << ++playerItemCount,
                Misc::StreamAsString() << name,
                Widget::NormalTextLayout::ALIGN_LEFT,
                Widget::WidgetRect(0.0, 0.0, 0.8, 0.03),
				Map::Teams::teamColor(worldSetup->getPlayerSettings(id)->getColor()));
            playerList->addChild(item);
        }
    }
    
    // set team counts
    for(int i = 0; i < Map::Teams::MAX_TEAMS; i++) {
        Widget::TextWidget *widget = dynamic_cast<Widget::TextWidget *>(
            lobby->getChild(Misc::StreamAsString() << "team-" << i << "-total"));
        
        //LOG(GUI, "Players on team " << i << ": " << worldSetup->getPlayersOnTeam(i));
        
        widget->setText(Misc::StreamAsString()
            << worldSetup->getPlayersOnTeam(i));
    }
}

void LobbyProxy::handleSwitchToScreen(Event::SwitchToScreen *event) {
    if(event->getScreen() == "lobby") {
        Widget::ButtonWidget *start =
            dynamic_cast<Widget::ButtonWidget *>(lobby->getChild("start"));
        if(start) {
            start->getText()->setText("Ready to start!");
        }
        
        // clear chat box
        Widget::ListWidget *chat = dynamic_cast<Widget::ListWidget *>(
            this->lobby->getChild("chatlist"));
        chat->removeAllChildren();
    }
}

LobbyProxy::LobbyProxy(Widget::WidgetBase *lobby) : lobby(lobby) {
    worldSetup = World::WorldSetup::getInstance();
    
    METHOD_OBSERVER(&LobbyProxy::handleSetupChat);
    METHOD_OBSERVER(&LobbyProxy::handleSetupPlayerSettings);
    METHOD_OBSERVER(&LobbyProxy::handleReplaceWorldSetup);
    METHOD_OBSERVER(&LobbyProxy::handleSwitchToScreen);
}

void LobbyProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen("-main"));
    }
    else if(name == "start") {
#if 0
        std::string map = Map::MapSettings::getInstance()->getMapFile();
        if(map != "") {
            LOG(GUI, "Using map \"" << map << "\"");
            
            Map::MapSettings::getInstance()->setGameType("starting");
            EMIT_EVENT(new Event::SwitchToScreen("loading"));
        }
        else {
            LOG2(GUI, WARNING, "No map selected!");
        }
#else
        int id = worldSetup->getClientID();
        World::WorldSetup::ClientSettings *settings
            = worldSetup->getClientSettings(id);
        if(settings) {
            settings->setReadyToStart(!settings->isReadyToStart());
            
            Widget::ButtonWidget *start =
                dynamic_cast<Widget::ButtonWidget *>(event->getWidget());
            if(start) {
                if(settings->isReadyToStart()) {
                    start->getText()->setText("No, I'm not ready...");
                }
                else {
                    start->getText()->setText("Ready now, really!");
                }
            }
            
            EMIT_EVENT(new Event::SetupClientSettings(*settings));
        }
#endif
    }
    else if(name.substr(0, 5) == "team-") {
        LOG(GUI, "Choosing " << name);
        
        std::istringstream sstream(name.substr(5));
        int col;
        sstream >> col;
        
        int id = worldSetup->getClientID();
        World::WorldSetup::PlayerSettings *settings
            = worldSetup->getPlayerSettings(id);
        if(settings) {
            settings->setColor(col);
            
            EMIT_EVENT(new Event::SetupPlayerSettings(*settings));
        }
        else {
            LOG2(GUI, WARNING, "Can't set colour");
        }
    }
    else {
        LOG2(GUI, WARNING, "No action for clicking on \"" << name << "\"");
    }
}

void LobbyProxy::visit(Widget::WidgetModifiedEvent *event) {
    std::string name = event->getWidget()->getName();
    std::string data = event->getWidget()->getData();
    
    if(name == "playername") {
        int id = worldSetup->getClientID();
        World::WorldSetup::PlayerSettings *settings
            = worldSetup->getPlayerSettings(id);
        if(settings) {
            if(data.length() == 0) {
                settings->setName(Object::Player::getDefaultPlayerName(id));
                event->getWidget()->setData(settings->getName());
                event->getWidget()->saveOldText();
            }
            else {
                settings->setName(data);
            }
            
            EMIT_EVENT(new Event::SetupPlayerSettings(*settings));
        }
        else {
            LOG2(GUI, WARNING, "Can't set player name to \"" << data << "\"");
        }
    }
    else if(name == "chat") {
        LOG(NETWORK, "Chat: " << data);
        event->getWidget()->setData("");
        event->getWidget()->saveOldText();
        
        int id = worldSetup->getClientID();
        std::string name = worldSetup->getPlayerSettings(id)->getName();
        EMIT_EVENT(new Event::SetupChat(id, name, data, true));
    }
    else {
        LOG2(GUI, WARNING, "No action for modifying \"" << name << "\"");
    }
}

void LobbyProxy::visit(Widget::WidgetSelectedEvent *event) {
    std::string name = event->getWidget()->getName();
    std::string select = event->getSelected()->getData();
    
    if(name == "playerlist") {
        // NYI
    }
    else {
        LOG2(GUI, WARNING, "No action for modifying \"" << name << "\"");
    }
}

void LobbyProxy::handleScrollbar(Widget::WidgetBase *scrollbar,
    Widget::WidgetActivateEvent *event) {
    
    std::string name = event->getWidget()->getName();
    if(name == "scrollbar-more") {
        int id = worldSetup->getClientID();
        
        // team-
        std::istringstream stream(scrollbar->getName().substr(5));
        int removeFrom;
        stream >> removeFrom;
        
        EMIT_EVENT(new Event::GeneralWorldSetupEvent(id,
            Misc::StreamAsString() << "remove-ai " << removeFrom));
    }
    else if(name == "scrollbar-less") {
        int id = worldSetup->getClientID();
        
        // team-
        std::istringstream stream(scrollbar->getName().substr(5));
        int addTo;
        stream >> addTo;
        
        EMIT_EVENT(new Event::GeneralWorldSetupEvent(id,
            Misc::StreamAsString() << "add-ai " << addTo));
    }
}

}  // namespace GUI
}  // namespace Project
