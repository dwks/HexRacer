#include <cctype>
#include "SDL_keysym.h"

#include "EditEventProxy.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "KeyEvent.h"
#include "WidgetActivateEvent.h"
#include "FocusEvent.h"

#include "FocusManager.h"

namespace Project {
namespace Widget {

void EditEventProxy::visit(MouseMoveEvent *event) {
    FocusManager::getInstance()->setMotionFocus(widget);
}

void EditEventProxy::visit(MouseButtonEvent *event) {
    if(event->getButton() == MouseButtonEvent::BUTTON_LEFT) {
        if(event->getDown()) {
            FocusManager::getInstance()->setClickFocus(widget);
        }
        else if(FocusManager::getInstance()->getClickFocus() == widget) {
            WidgetActivateEvent newEvent(widget, true);
            
            widget->handleEvent(&newEvent);
        }
        
        if(!event->getDown()) {
            FocusManager::getInstance()->setClickFocus(NULL);
        }
    }
}

void EditEventProxy::visit(KeyEvent *event) {
    if(event->getUnicode() && std::isprint(event->getUnicode())) {
        // unicode translation enabled, got a printable key
        
        widget->addCharacter(static_cast<char>(event->getUnicode()));
    }
    else if(event->getDown() && event->getKey() == SDLK_BACKSPACE) {
        std::string oldText = widget->getData();
        
        if(oldText.length() > 0) {
            widget->setData(oldText.substr(0, oldText.length() - 1));
        }
    }
}

void EditEventProxy::visit(WidgetActivateEvent *event) {
    
}

void EditEventProxy::visit(FocusEvent *event) {
    FocusManager *focus = FocusManager::getInstance();
    bool motionFocus = (focus->getMotionFocus() == widget);
    bool clickFocus = (focus->getClickFocus() == widget);
    
    if(event->getFocus() == FocusEvent::FOCUS_MOTION && event->wasLost()) {
        motionFocus = false;
    }
    if(event->getFocus() == FocusEvent::FOCUS_CLICK && event->wasLost()) {
        clickFocus = false;
    }
    
    if(clickFocus) {
        widget->getBox()->setArtwork("corners/in/active");
    }
    else if(motionFocus) {
        widget->getBox()->setArtwork("corners/in/hover");
    }
    else {
        widget->getBox()->setArtwork("corners/in/normal");
    }
}

}  // namespace Widget
}  // namespace Project
