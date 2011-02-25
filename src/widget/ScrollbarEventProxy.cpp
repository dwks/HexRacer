#include "ScrollbarEventProxy.h"
#include "ScrollbarWidget.h"

#include "FocusManager.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void ScrollbarEventProxy::visit(MouseMoveEvent *event) {
    Widget::WidgetBase *child[4];
    child[0] = widget->getLess();
    child[1] = widget->getMore();
    child[2] = widget->getSlider();  // slider before bar
    child[3] = widget->getBar();
    
    for(std::size_t x = 0; x < sizeof child / sizeof *child; x ++) {
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

void ScrollbarEventProxy::visit(MouseButtonEvent *event) {
    Widget::WidgetBase *child[4];
    child[0] = widget->getLess();
    child[1] = widget->getMore();
    child[2] = widget->getSlider();  // slider before bar
    child[3] = widget->getBar();
    
    for(std::size_t x = 0; x < sizeof child / sizeof *child; x ++) {
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

}  // namespace Widget
}  // namespace Project
