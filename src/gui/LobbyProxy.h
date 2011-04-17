#ifndef PROJECT_GUI__LOBBY_PROXY_H
#define PROJECT_GUI__LOBBY_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/SetupChat.h"
#include "event/ReplaceWorldSetup.h"
#include "event/SetupPlayerSettings.h"

#include "world/WorldSetup.h"

namespace Project {
namespace GUI {

class LobbyProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void handleSetupChat(Event::SetupChat *event);
    void handleSetupPlayerSettings(Event::SetupPlayerSettings *event);
    void handleReplaceWorldSetup(Event::ReplaceWorldSetup *event);
private:
    Widget::WidgetBase *lobby;
    World::WorldSetup *worldSetup;
public:
    class ScrollbarProxy : public Widget::EventProxy {
    private:
        LobbyProxy *lobbyProxy;
        Widget::WidgetBase *scrollbar;
    public:
        ScrollbarProxy(LobbyProxy *lobbyProxy, Widget::WidgetBase *scrollbar)
            : lobbyProxy(lobbyProxy), scrollbar(scrollbar) {}
        
        virtual void visit(Widget::WidgetActivateEvent *event)
            { lobbyProxy->handleScrollbar(scrollbar, event); }
    };
public:
    LobbyProxy(Widget::WidgetBase *lobby);
    
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
protected:
    void handleScrollbar(Widget::WidgetBase *scrollbar,
        Widget::WidgetActivateEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
