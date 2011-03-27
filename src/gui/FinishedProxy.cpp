#include "FinishedProxy.h"

#include "widget/WidgetActivateEvent.h"
#include "widget/CompositeWidget.h"

#include "event/EventSystem.h"
#include "event/SwitchToScreen.h"
#include "event/QuitEvent.h"

namespace Project {
namespace GUI {

void FinishedProxy::handleRaceFinished(Event::RaceFinished *event) {
    LOG(GLOBAL, "Race finished!");
    
    Widget::CompositeWidget *ranks = dynamic_cast<Widget::CompositeWidget *>(
        finished->getChild("ranks"));
    ranks->removeAllChildren();
    
    Map::RaceResults results = event->getResults();
    for(int r = 0; r < results.getRanks() && r < 10; r ++) {
        
    }
    
    EMIT_EVENT(new Event::SwitchToScreen("finished"));
}

FinishedProxy::FinishedProxy(Widget::WidgetBase *finished) {
    METHOD_OBSERVER(&FinishedProxy::handleRaceFinished);
}

void FinishedProxy::visit(Widget::WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "quit") {
        EMIT_EVENT(new Event::QuitEvent());
    }
}

}  // namespace GUI
}  // namespace Project
