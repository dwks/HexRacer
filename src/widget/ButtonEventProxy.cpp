#include "ButtonEventProxy.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void ButtonEventProxy::visit(MouseMoveEvent *event) {
    
}

void ButtonEventProxy::visit(MouseButtonEvent *event) {
    LOG(WIDGET, "Button " << widget->getName()
        << " clicked at " << event->getWhere());
}

}  // namespace Widget
}  // namespace Project
