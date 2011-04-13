#include "ListItemEventProxy.h"

#include "WidgetBase.h"
#include "ListWidget.h"

#include "MouseButtonEvent.h"
#include "WidgetSelectedEvent.h"

#include "FocusManager.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void ListItemEventProxy::visit(MouseButtonEvent *event) {
    if(event->getButton() == MouseButtonEvent::BUTTON_LEFT) {
        if(event->getDown()) {
            FocusManager::getInstance()->setClickFocus(text);
        }
        else if(FocusManager::getInstance()->getClickFocus() == text) {
            WidgetSelectedEvent newEvent(list, text);
            
            LOG(WIDGET, "Item \"" << text->getName()
                << "\" selected in \"" << list->getName() << '"');
            
            list->setLastSelected(text);
            list->handleEvent(&newEvent);
        }
        
        if(!event->getDown()) {
            FocusManager::getInstance()->setClickFocus(NULL);
        }
    }
}

}  // namespace Widget
}  // namespace Project
