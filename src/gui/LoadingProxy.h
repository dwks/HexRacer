#ifndef PROJECT_GUI__LOADING_PROXY_H
#define PROJECT_GUI__LOADING_PROXY_H

#include "widget/EventProxy.h"
#include "widget/WidgetBase.h"

#include "event/Enabler.h"
#include "event/SwitchToScreen.h"
#include "event/GameStageChanged.h"

namespace Project {
namespace GUI {

class LoadingProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void initialize(Event::SwitchToScreen *event);
    void gameStateChanged(Event::GameStageChanged *event);
private:
    Widget::WidgetBase *loading, *host;
    Widget::WidgetBase *lastRepaint;
public:
    LoadingProxy(Widget::WidgetBase *loading, Widget::WidgetBase *host);
    
    virtual void visit(Widget::RepaintEvent *event);
private:
    void announceFinishedLoading();
};

}  // namespace GUI
}  // namespace Project

#endif
