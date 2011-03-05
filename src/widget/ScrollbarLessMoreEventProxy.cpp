#include "ScrollbarLessMoreEventProxy.h"
#include "ScrollbarWidget.h"

#include "WidgetActivateEvent.h"

namespace Project {
namespace Widget {

void ScrollbarLessMoreEventProxy::visit(WidgetActivateEvent *event) {
    std::string name = event->getWidget()->getName();
    
    if(name == "scrollbar-less") {
        widget->addValue(-widget->getSize() / 10.0);
    }
    else if(name == "scrollbar-more") {
        widget->addValue(+widget->getSize() / 10.0);
    }
}

}  // namespace Widget
}  // namespace Project
