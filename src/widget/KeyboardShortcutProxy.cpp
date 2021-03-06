#include "KeyboardShortcutProxy.h"
#include "KeyEvent.h"
#include "WidgetActivateEvent.h"
#include "FocusManager.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void KeyboardShortcutProxy::visit(Widget::KeyEvent *event) {
    if(FocusManager::getInstance()->getKeyFocus() != NULL) return;
    
    if(event->getKey() == key && event->getDown()) {
        LOG(WIDGET, "Keyboard shortcut for \""
            << widget->getName() << "\" pressed");
        
        WidgetActivateEvent newEvent(widget, true);
        widget->handleEvent(&newEvent);
    }
}

}  // namespace Widget
}  // namespace Project
