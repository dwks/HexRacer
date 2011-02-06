#include "CompositeEventProxy.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "KeyEvent.h"

#include "FocusManager.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void CompositeEventProxy::visit(MouseMoveEvent *event) {
    bool handled = false;
    
    CompositeWidget::IteratorType i = widget->getIterator();
    while(i.hasNext()) {
        WidgetBase *child = i.next();
        
        if(child->getBoundingRect().pointInside(event->getWhere())) {
            child->handleEvent(event);
            handled = true;
        }
    }
    
    if(!handled) {
        FocusManager::getInstance()->setMotionFocus(NULL);
    }
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
