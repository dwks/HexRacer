#include "ListScrollbarEventProxy.h"
#include "ListWidget.h"

namespace Project {
namespace Widget {

void ListScrollbarEventProxy::visit(MouseMoveEvent *event) {
    list->repositionChildren();
}

}  // namespace Widget
}  // namespace Project
