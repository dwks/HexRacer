#include "ListWidgetBoxProxy.h"
#include "MouseButtonEvent.h"

#include "ListWidget.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

ListWidgetBoxProxy::ListWidgetBoxProxy(ListWidget *list) : list(list) {
    
}

void ListWidgetBoxProxy::visit(MouseButtonEvent *event) {
    WidgetRect area = list->getViewArea();
    
    //LOG(WIDGET, event->getWhere() << " inside " << area << " ?");
    
    // check for clicks to the right of list items
    if(area.pointInside(event->getWhere())) {
        CompositeWidget::IteratorType it = list->getIterator();
        while(it.hasNext()) {
            WidgetBase *child = it.next();
            
            WidgetRect rect = child->getBoundingRect();
            
            if(event->getWhere().getY() >= rect.getCorner().getY()
                && event->getWhere().getY() <= rect.getCorner().getY() + rect.getHeight()) {
                
                child->handleEvent(event);
                return;
            }
        }
    }
}

}  // namespace Widget
}  // namespace Project
