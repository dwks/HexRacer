#ifndef PROJECT_GUI__SINGLE_PLAYER_PROXY_H
#define PROJECT_GUI__SINGLE_PLAYER_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

namespace Project {
namespace GUI {

class SinglePlayerProxy : public Widget::EventProxy {
private:
    Widget::WidgetBase *singleplayer;
public:
    SinglePlayerProxy(Widget::WidgetBase *singleplayer)
        : singleplayer(singleplayer) {}
    
    virtual void visit(Widget::WidgetActivateEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
