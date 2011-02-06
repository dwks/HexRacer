#include "CompositeEventProxy.h"

#include "MouseButtonEvent.h"
#include "KeyEvent.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void CompositeEventProxy::visit(MouseMoveEvent *event) {
    // nyi
}

void CompositeEventProxy::visit(MouseButtonEvent *event) {
    CompositeWidget::IteratorType i = widget->getIterator();
    while(i.hasNext()) {
        WidgetBase *child = i.next();
        
        if(child->getBoundingRect().pointInside(event->getWhere())) {
            child->handleEvent(event);
        }
    }
}

void CompositeEventProxy::visit(KeyEvent *event) {
    CompositeWidget::IteratorType i = widget->getIterator();
    while(i.hasNext()) {
        WidgetBase *child = i.next();
        
        child->handleEvent(event);
    }
}

}  // namespace Widget
}  // namespace Project
