#include "ScrollbarSliderEventProxy.h"
#include "ScrollbarWidget.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"

#include "FocusManager.h"
#include "FocusEvent.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void ScrollbarSliderEventProxy::visit(MouseMoveEvent *event) {
    BoxWidget *bar = scrollbar->getBar();
    BoxWidget *slider = scrollbar->getSlider();
    
    FocusManager::getInstance()->setMotionFocus(slider);
    
    if(sliding) {
        WidgetPoint difference = event->getWhere() - lastSlidePosition;
        if(scrollbar->isVertical()) {
            double mouse = difference.getY();
            double change = mouse / bar->getBoundingRect().getHeight();
            
            scrollbar->addValue(change);
        }
        else {
            double mouse = difference.getX();
            double change = mouse / bar->getBoundingRect().getWidth();
            
            scrollbar->addValue(change);
        }
        
        lastSlidePosition = event->getWhere();
    }
}

void ScrollbarSliderEventProxy::visit(MouseButtonEvent *event) {
    WidgetBase *widget = scrollbar->getSlider();
    
    if(event->getButton() == MouseButtonEvent::BUTTON_LEFT) {
        if(event->getDown()) {
            FocusManager::getInstance()->setClickFocus(widget);
            
            sliding = true;
            lastSlidePosition = event->getWhere();
        }
        else {
            sliding = false;
            
            FocusManager::getInstance()->setClickFocus(NULL);
        }
    }
}

void ScrollbarSliderEventProxy::visit(FocusEvent *event) {
    BoxWidget *widget = scrollbar->getSlider();
    
    FocusManager *focus = FocusManager::getInstance();
    bool motionFocus = (focus->getMotionFocus() == widget);
    bool clickFocus = (focus->getClickFocus() == widget);
    
    if(event->getFocus() == FocusEvent::FOCUS_MOTION && event->wasLost()) {
        motionFocus = false;
    }
    if(event->getFocus() == FocusEvent::FOCUS_CLICK && event->wasLost()) {
        sliding = false;
        clickFocus = false;
    }
    
    if(clickFocus) {
        widget->setArtwork("corners/out/active");
    }
    else if(motionFocus) {
        widget->setArtwork("corners/out/hover");
    }
    else {
        widget->setArtwork("corners/out/normal");
    }
}

}  // namespace Widget
}  // namespace Project
