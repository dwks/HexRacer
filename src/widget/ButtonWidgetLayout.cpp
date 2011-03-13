#include "ButtonWidgetLayout.h"

#include "ButtonWidget.h"

namespace Project {
namespace Widget {

ButtonWidgetLayout::ButtonWidgetLayout(ButtonWidget *widget,
    const WidgetRect &bounds) : widget(widget) {
    
    update(bounds);
}

void ButtonWidgetLayout::update() {
    // nothing to do
}

void ButtonWidgetLayout::update(const WidgetRect &newBounds) {
    widget->getBox()->updateLayout(newBounds);
    
    WidgetRect textBounds = newBounds;
    textBounds.getCorner() += textBounds.getDimensions() * 0.15;
    textBounds.getDimensions() *= 0.7;
    widget->getText()->updateLayout(textBounds);
    
    this->bounds = newBounds;
}

WidgetRect ButtonWidgetLayout::getBoundingRect() const {
    return bounds;
}

}  // namespace Widget
}  // namespace Project
