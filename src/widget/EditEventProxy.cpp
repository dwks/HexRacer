#include <cctype>
#include "SDL_keysym.h"

#include "EditEventProxy.h"

#include "MouseMoveEvent.h"
#include "MouseButtonEvent.h"
#include "KeyEvent.h"
#include "WidgetActivateEvent.h"
#include "FocusEvent.h"
#include "WidgetModifiedEvent.h"

#include "FocusManager.h"

#include "log/Logger.h"

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
    if(FocusManager::getInstance()->getKeyFocus() != widget) return;
    
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
    else if(event->getDown() && event->getKey() == SDLK_ESCAPE) {
        widget->restoreOldText();
        FocusManager::getInstance()->setKeyFocus(NULL);
    }
    else if(event->getDown() && event->getKey() == SDLK_RETURN) {
        bool changed = widget->oldTextChanged();
        widget->saveOldText();
        FocusManager::getInstance()->setKeyFocus(NULL);
        
        if(changed) {
            WidgetModifiedEvent newEvent(widget);
            
            LOG(WIDGET, "Edit \"" << widget->getName()
                << "\" has text \"" << widget->getData() << "\"");
            
            widget->handleEvent(&newEvent);
        }
    }
}

void EditEventProxy::visit(WidgetActivateEvent *event) {
    FocusManager::getInstance()->setKeyFocus(widget);
}

void EditEventProxy::visit(FocusEvent *event) {
    FocusManager *focus = FocusManager::getInstance();
    bool motionFocus = (focus->getMotionFocus() == widget);
    bool clickFocus = (focus->getClickFocus() == widget);
    bool keyFocus = (focus->getKeyFocus() == widget);
    
    if(event->getFocus() == FocusEvent::FOCUS_MOTION && event->wasLost()) {
        motionFocus = false;
    }
    if(event->getFocus() == FocusEvent::FOCUS_CLICK && event->wasLost()) {
        clickFocus = false;
    }
    if(event->getFocus() == FocusEvent::FOCUS_KEY && event->wasLost()) {
        keyFocus = false;
    }
    
    if(clickFocus || keyFocus) {
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
