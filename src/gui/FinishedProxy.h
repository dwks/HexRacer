#ifndef PROJECT_GUI__FINISHED_PROXY_H
#define PROJECT_GUI__FINISHED_PROXY_H

#include "widget/WidgetBase.h"
#include "widget/EventProxy.h"

#include "event/Enabler.h"
#include "event/RaceFinished.h"

namespace Project {
namespace GUI {

class FinishedProxy : public Widget::EventProxy, public Event::Enabler {
protected:
    void handleRaceFinished(Event::RaceFinished *event);
private:
    Widget::WidgetBase *finished;
public:
    FinishedProxy(Widget::WidgetBase *finished);
    
    virtual void visit(Widget::WidgetActivateEvent *event);
};

}  // namespace GUI
}  // namespace Project

#endif
