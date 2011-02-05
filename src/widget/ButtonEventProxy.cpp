#include "ButtonEventProxy.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "WidgetActivateEvent.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void ButtonEventProxy::visit(MouseMoveEvent *event) {
    
}

void ButtonEventProxy::visit(MouseButtonEvent *event) {
    if(event->getButton() == MouseButtonEvent::BUTTON_LEFT) {
        if(event->getDown()) {
            // nothing yet
        }
        else {
            WidgetActivateEvent newEvent(widget, true);
            
            LOG(WIDGET, "Button \"" << widget->getName()
                << "\" clicked at " << event->getWhere());
            
            widget->handleEvent(&newEvent);
        }
    }
}

}  // namespace Widget
}  // namespace Project
