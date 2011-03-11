#include "CheckEventProxy.h"
#include "CheckWidget.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "WidgetActivateEvent.h"
#include "FocusEvent.h"

#include "FocusManager.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void CheckEventProxy::visit(MouseMoveEvent *event) {
    FocusManager::getInstance()->setMotionFocus(widget);
}

void CheckEventProxy::visit(MouseButtonEvent *event) {
    if(event->getButton() == MouseButtonEvent::BUTTON_LEFT) {
        if(event->getDown()) {
            FocusManager::getInstance()->setClickFocus(widget);
        }
        else if(FocusManager::getInstance()->getClickFocus() == widget
            && widget->getBoundingRect().pointInside(event->getWhere())) {
            
            widget->toggleChecked();
            
			WidgetActivateEvent newEvent(widget, widget->isChecked());
            
            LOG(WIDGET, "Checkbox \"" << widget->getName()
                << "\" toggled at " << event->getWhere());
            
            widget->handleEvent(&newEvent);
        }
        
        if(!event->getDown()) {
            FocusManager::getInstance()->setClickFocus(NULL);
        }
    }
}

void CheckEventProxy::visit(FocusEvent *event) {
    FocusManager *focus = FocusManager::getInstance();
    bool motionFocus = (focus->getMotionFocus() == widget);
    bool clickFocus = (focus->getClickFocus() == widget);
    
    if(event->getFocus() == FocusEvent::FOCUS_MOTION && event->wasLost()) {
        motionFocus = false;
    }
    if(event->getFocus() == FocusEvent::FOCUS_CLICK && event->wasLost()) {
        clickFocus = false;
    }
    
    if(!widget->isChecked()) {
        if(clickFocus && motionFocus) {
            widget->getBox()->getBox()->setArtwork("corners/in/active");
        }
        else if(clickFocus) {
            widget->getBox()->getBox()->setArtwork("corners/out/active");
        }
        else if(motionFocus) {
            widget->getBox()->getBox()->setArtwork("corners/out/hover");
        }
        else {
            widget->getBox()->getBox()->setArtwork("corners/out/normal");
        }
    }
    else {
        if(clickFocus && motionFocus) {
            widget->getBox()->getBox()->setArtwork("corners/out/active");
        }
        else if(clickFocus) {
            widget->getBox()->getBox()->setArtwork("corners/in/active");
        }
        else if(motionFocus) {
            widget->getBox()->getBox()->setArtwork("corners/in/hover");
        }
        else {
            widget->getBox()->getBox()->setArtwork("corners/in/normal");
        }
    }
}

}  // namespace Widget
}  // namespace Project
