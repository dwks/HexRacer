#include "ScrollbarWidget.h"

#include "AbsoluteLayout.h"
#include "NormalTextLayout.h"
#include "ScrollbarLayout.h"

#include "ScrollbarEventProxy.h"
#include "ScrollbarSliderEventProxy.h"
#include "ScrollbarLessMoreEventProxy.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

ScrollbarWidget::ScrollbarWidget(const std::string &name, bool vertical,
    const WidgetRect &bounds)
    : AbstractWidget(name), vertical(vertical) {
    
    less = new ButtonWidget("scrollbar-less", vertical ? "^" : "<", bounds);
    more = new ButtonWidget("scrollbar-more", vertical ? "v" : ">", bounds);
    bar = new BoxWidget("scrollbar-bar", "corners/in/normal");
    slider = new BoxWidget("scrollbar-slider");
    
    less->setLayout(new AbsoluteLayout(bounds));
    more->setLayout(new AbsoluteLayout(bounds));
    bar->setLayout(new AbsoluteLayout(bounds));
    slider->setLayout(new AbsoluteLayout(bounds));
    
    EventProxy *lessMoreProxy = new ScrollbarLessMoreEventProxy(this);
    less->addEventProxy(lessMoreProxy);
    more->addEventProxy(lessMoreProxy);
    slider->addEventProxy(new ScrollbarSliderEventProxy(this));
    
    setLayout(new ScrollbarLayout(this, bounds));
    addEventProxy(new ScrollbarEventProxy(this));
    
    // !!! testing
    //setEverything(0.4, 0.1, 1.0);
    
    // force the bar and slider to be updated
    updateLayout();
}

void ScrollbarWidget::setValue(double value) {
    this->value = value;
    ensureValid();
    updateScrolling();
}

void ScrollbarWidget::setSize(double size) {
    this->size = size;
    ensureValid();
    updateScrolling();
}

void ScrollbarWidget::setMax(double max, bool scaleExisting) {
    if(scaleExisting) {
        value *= (max / this->max);
        size *= (max / this->max);
    }
    
    this->max = max;
    ensureValid();
    updateScrolling();
}

void ScrollbarWidget::setEverything(double value, double size, double max) {
    this->value = value;
    this->size = size;
    this->max = max;
    ensureValid();
    updateScrolling();
}

void ScrollbarWidget::addValue(double add) {
    this->value += add;
    ensureValid();
    updateScrolling();
}

void ScrollbarWidget::render(WidgetVisitor &renderer) {
    less->accept(renderer);
    more->accept(renderer);
    
    bar->accept(renderer);
    slider->accept(renderer);
}

void ScrollbarWidget::ensureValid() {
    // validate max
    if(max < 0.0) {
        LOG2(WIDGET, WARNING, "Scrollbar max " << max
            << " is negative, setting to zero");
        max = 0.0;
    }
    
    // validate size
    if(size < 0.0) {
        LOG2(WIDGET, WARNING, "Scrollbar size " << size
            << " is negative, setting to zero");
        size = 0.0;
    }
    if(size > max) {
        LOG2(WIDGET, WARNING, "Scrollbar size " << size
            << " is larger than max " << max);
        size = max;
    }
    
    // validate value
    if(value < 0.0) {
        /*LOG2(WIDGET, WARNING, "Scrollbar value " << value
            << " is negative, setting to zero");*/
        value = 0.0;
    }
    if(value + size > max) {
        /*LOG2(WIDGET, WARNING, "Scrollbar value " << value
            << " is too large, clipping");*/
        value = max - size;
    }
}

void ScrollbarWidget::updateScrolling() {
    double from = value / max;
    double to = (value + size) / max;
    
    dynamic_cast<ScrollbarLayout *>(getLayout().get())->setViewport(from, to);
    updateLayout();
}

}  // namespace Widget
}  // namespace Project
