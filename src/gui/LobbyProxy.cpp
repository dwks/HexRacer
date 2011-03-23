#include "LobbyProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/WidgetModifiedEvent.h"
#include "widget/WidgetSelectedEvent.h"
#include "widget/EditWidget.h"
#include "widget/ListWidget.h"

#include "event/EventSystem.h"
#include "event/SwitchToScreen.h"

#include "map/MapSettings.h"

#include "settings/SettingsManager.h"
#include "misc/StreamAsString.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void LobbyProxy::handleSetupChat(Event::SetupChat *event) {
    static int chatCount = 0;
    
    int id = event->getClient();
    std::string name = event->getName();
    std::string message = event->getMessage();
    
    Widget::TextWidget *item = new Widget::TextWidget(
        Misc::StreamAsString() << "chat" << ++chatCount,
        Misc::StreamAsString() << name << ": " << message,
        Widget::NormalTextLayout::ALIGN_LEFT,
        Widget::WidgetRect(0.0, 0.0, 0.8, 0.03));
    
    item->setColor(worldSetup->getPlayerSettings(id)->getColor());
    
    Widget::ListWidget *chat = dynamic_cast<Widget::ListWidget *>(
        this->lobby->getChild("chat"));
    if(chat) {
        chat->addChild(item);
    }
    else {
        LOG2(GUI, ERROR, "Can't find chat widget to add message to");
    }
}

LobbyProxy::LobbyProxy(Widget::WidgetBase *lobby)
    : lobby(lobby), worldSetup(0) {
    
    METHOD_OBSERVER(&LobbyProxy::handleSetupChat);
}

void LobbyProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "cancel") {
        EMIT_EVENT(new Event::SwitchToScreen("-main"));
    }
    else if(name == "start") {
        std::string map = Map::MapSettings::getInstance()->getMapFile();
        if(map != "") {
            LOG(GUI, "Using map \"" << map << "\"");
            
            Map::MapSettings::getInstance()->setGameType("starting");
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

void LobbyProxy::visit(Widget::WidgetModifiedEvent *event) {
    std::string name = event->getWidget()->getName();
    std::string data = event->getWidget()->getData();
    
    if(name == "playername") {
        LOG2(GUI, WARNING, "Setting player name to \"" << data << "\" -- NYI");
    }
    else if(name == "chat") {
        LOG(NETWORK, "Chat: " << data);
        event->getWidget()->setData("");
        
        int id = worldSetup->getClientID();
        std::string name = worldSetup->getPlayerSettings(id)->getName();
        EMIT_EVENT(new Event::SetupChat(id, name, data));
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
    else if(name == "colourlist") {
        // NYI
    }
    else {
        LOG2(GUI, WARNING, "No action for modifying \"" << name << "\"");
    }
}

}  // namespace GUI
}  // namespace Project
