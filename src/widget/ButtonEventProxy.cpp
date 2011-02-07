#include "ButtonEventProxy.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "WidgetActivateEvent.h"
#include "FocusEvent.h"

#include "FocusManager.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void ButtonEventProxy::visit(MouseMoveEvent *event) {
    FocusManager::getInstance()->setMotionFocus(widget);
    
    //widget->getBox()->setArtwork("corners/out/hover");
}

void ButtonEventProxy::visit(MouseButtonEvent *event) {
    if(event->getButton() == MouseButtonEvent::BUTTON_LEFT) {
        if(event->getDown()) {
            FocusManager::getInstance()->setClickFocus(widget);
            //widget->getBox()->setArtwork("corners/in/active");
        }
        else if(FocusManager::getInstance()->getClickFocus() == widget) {
            //widget->getBox()->setArtwork("corners/out/normal");
            
            WidgetActivateEvent newEvent(widget, true);
            
            LOG(WIDGET, "Button \"" << widget->getName()
                << "\" clicked at " << event->getWhere());
            
            widget->handleEvent(&newEvent);
        }
        
        if(!event->getDown()) {
            FocusManager::getInstance()->setClickFocus(NULL);
        }
    }
}

void ButtonEventProxy::visit(FocusEvent *event) {
    FocusManager *focus = FocusManager::getInstance();
    bool motionFocus = ( focus->getMotionFocus() == widget);
    bool clickFocus = (focus->getClickFocus() == widget);
    
    if(event->getFocus() == FocusEvent::FOCUS_MOTION && event->wasLost()) {
        motionFocus = false;
    }
    if(event->getFocus() == FocusEvent::FOCUS_CLICK && event->wasLost()) {
        clickFocus = false;
    }
    
    if(clickFocus && motionFocus) {
        widget->getBox()->setArtwork("corners/in/active");
    }
    else if(clickFocus) {
        widget->getBox()->setArtwork("corners/out/active");
    }
    else if(motionFocus) {
        widget->getBox()->setArtwork("corners/out/hover");
    }
    else {
        widget->getBox()->setArtwork("corners/out/normal");
    }
    
    /*if(event->getFocus() == FocusEvent::FOCUS_MOTION) {
        if(event->wasGained()) {
            widget->getBox()->setArtwork("corners/out/hover");
        }
        else {
            widget->getBox()->setArtwork("corners/out/normal");
        }
    }*/
}

}  // namespace Widget
}  // namespace Project
