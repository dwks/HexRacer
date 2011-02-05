#include "CompositeEventProxy.h"

#include "WidgetActivateEvent.h"

namespace Project {
namespace Widget {

void CompositeEventProxy::visit(MouseMoveEvent *event) {
    // nyi
}

void CompositeEventProxy::visit(WidgetActivateEvent *event) {
    if(proxy) {
        proxy->handleEvent(event);
    }
}

}  // namespace Widget
}  // namespace Project
