#ifndef PROJECT_GUI__LOBBY_PROXY_H
#define PROJECT_GUI__LOBBY_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

namespace Project {
namespace GUI {

class LobbyProxy : public Widget::EventProxy {
private:
    Widget::WidgetBase *lobby;
public:
    LobbyProxy(Widget::WidgetBase *lobby) : lobby(lobby) {}
    
    virtual void visit(Widget::WidgetActivateEvent *event);
    virtual void visit(Widget::WidgetModifiedEvent *event);
    virtual void visit(Widget::WidgetSelectedEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
