#include "ListEventProxy.h"
#include "ListWidget.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "FocusEvent.h"

#include "FocusManager.h"

namespace Project {
namespace Widget {

void ListEventProxy::visit(MouseMoveEvent *event) {
    Widget::WidgetBase *child[3];
    child[0] = widget->getBox();
    child[1] = widget->getHorizontalBar();
    child[2] = widget->getVerticalBar();
    
    for(std::size_t x = 0; x < sizeof child / sizeof *child; x ++) {
        if(!child[x]) continue;
        
        if(FocusManager::getInstance()->getMotionFocus() == child[x]
            || FocusManager::getInstance()->getClickFocus() == child[x]) {
            
            child[x]->handleEvent(event);
            return;
        }
        
        if(child[x]->getBoundingRect().pointInside(event->getWhere())) {
            child[x]->handleEvent(event);
            return;
        }
    }
}

void ListEventProxy::visit(MouseButtonEvent *event) {
    Widget::WidgetBase *child[3];
    child[0] = widget->getBox();
    child[1] = widget->getHorizontalBar();
    child[2] = widget->getVerticalBar();
    
    for(std::size_t x = 0; x < sizeof child / sizeof *child; x ++) {
        if(!child[x]) continue;
        
        if(FocusManager::getInstance()->getClickFocus() == child[x]) {
            child[x]->handleEvent(event);
            return;
        }
        
        if(child[x]->getBoundingRect().pointInside(event->getWhere())
            && event->getDown()) {
            
            child[x]->handleEvent(event);
            return;
        }
    }
    
    FocusManager::getInstance()->setClickFocus(NULL);
}

void ListEventProxy::visit(FocusEvent *event) {
    
}

}  // namespace Widget
}  // namespace Project
