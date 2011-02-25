#include "ListWidget.h"

namespace Project {
namespace Widget {

ListWidget::ListWidget(const std::string &name, int lines,
    bool vertical, bool horizontal, const WidgetRect &bounds)
    : CompositeWidget(name), lines(lines) {
    
    verticalBar = 0;
    horizontalBar = 0;
    
    if(vertical) {
        //verticalBar = new ScrollbarWidget();
    }
    if(horizontal) {
        //horizontalBar = new ScrollbarWidget();
    }
}

void ListWidget::addChild(WidgetBase *widget) {
    
}

}  // namespace Widget
}  // namespace Project
