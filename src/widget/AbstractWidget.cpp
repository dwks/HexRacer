#include "AbstractWidget.h"
#include "Layout.h"

namespace Project {
namespace Widget {

AbstractWidget::~AbstractWidget() {
    delete layout;
}

void AbstractWidget::updateLayout() {
    layout->update();
}

void AbstractWidget::updateLayout(const WidgetRect &newBounds) {
    layout->update(newBounds);
}

WidgetRect AbstractWidget::getBoundingRect() const {
    return layout->getBoundingRect();
}

}  // namespace Widget
}  // namespace Project
