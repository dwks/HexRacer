#include "FocusManager.h"
#include "FocusEvent.h"

namespace Project {
namespace Widget {

FocusManager *FocusManager::instance = 0;

FocusManager::FocusManager() {
    instance = this;
    
    motionFocus = NULL;
    clickFocus = NULL;
    keyFocus = NULL;
}

void FocusManager::setMotionFocus(WidgetBase *focus) {
    if(motionFocus == focus) return;
    
    WidgetBase *oldFocus = motionFocus;
    
    FocusEvent focusLost(FocusEvent::FOCUS_MOTION, false);
    if(oldFocus) oldFocus->handleEvent(&focusLost);
    
    motionFocus = focus;
    
    FocusEvent focusGained(FocusEvent::FOCUS_MOTION, true);
    if(focus) focus->handleEvent(&focusGained);
}

void FocusManager::setClickFocus(WidgetBase *focus) {
    if(clickFocus == focus) return;
    
    WidgetBase *oldFocus = clickFocus;
    
    FocusEvent focusLost(FocusEvent::FOCUS_CLICK, false);
    if(oldFocus) oldFocus->handleEvent(&focusLost);
    
    clickFocus = focus;
    
    FocusEvent focusGained(FocusEvent::FOCUS_CLICK, true);
    if(focus) focus->handleEvent(&focusGained);
}

void FocusManager::setKeyFocus(WidgetBase *focus) {
    if(keyFocus == focus) return;
    
    WidgetBase *oldFocus = keyFocus;
    
    FocusEvent focusLost(FocusEvent::FOCUS_KEY, false);
    if(oldFocus) oldFocus->handleEvent(&focusLost);
    
    keyFocus = focus;
    
    FocusEvent focusGained(FocusEvent::FOCUS_KEY, true);
    if(focus) focus->handleEvent(&focusGained);
}

}  // namespace Widget
}  // namespace Project
