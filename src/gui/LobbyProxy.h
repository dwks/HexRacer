#ifndef PROJECT_GUI__LOBBY_PROXY_H
#define PROJECT_GUI__LOBBY_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/SetupChat.h"

#include "world/WorldSetup.h"

namespace Project {
namespace GUI {

class LobbyProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void handleSetupChat(Event::SetupChat *event);
private:
    Widget::WidgetBase *lobby;
    World::WorldSetup *worldSetup;
public:
    LobbyProxy(Widget::WidgetBase *lobby);
    
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
