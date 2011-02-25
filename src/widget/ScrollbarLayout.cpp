#include "ScrollbarLayout.h"
#include "ScrollbarWidget.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void ScrollbarLayout::setViewport(double from, double to) {
    this->from = from;
    this->to = to;
}

void ScrollbarLayout::update() {
    // first set the size of less and more
    
    WidgetRect lessRect = bounds;
    WidgetRect moreRect = bounds;
    
    if(widget->isVertical()) {
        lessRect.getDimensions().setY(bounds.getWidth());
        
        moreRect.getDimensions().setY(bounds.getWidth());
        moreRect.getCorner().addY(bounds.getHeight() - moreRect.getHeight());
    }
    else {
        lessRect.getDimensions().setX(bounds.getHeight());
        
        moreRect.getDimensions().setX(bounds.getHeight());
        moreRect.getCorner().addX(bounds.getWidth() - moreRect.getWidth());
    }
    
    widget->getLess()->updateLayout(lessRect);
    widget->getMore()->updateLayout(moreRect);
    widget->getLess()->updateLayout();
    widget->getMore()->updateLayout();
    
    /*LOG(WIDGET, "scrollbar less is " << lessRect
        << ", more is " << moreRect);*/
    
    // now figure out where to position bar and slider
    
    WidgetPoint upperLeft = bounds.getCorner();
    WidgetPoint dimensions = bounds.getDimensions();
    
    if(widget->isVertical()) {
        double minY = lessRect.getCorner().getY() + lessRect.getHeight();
        dimensions.addY(-(minY - upperLeft.getY()));
        upperLeft.setY(minY);
        
        double maxY = moreRect.getCorner().getY();
        dimensions.addY(-((upperLeft.getY() + dimensions.getY()) - maxY));
    }
    else {
        double minX = lessRect.getCorner().getX() + lessRect.getWidth();
        dimensions.addX(-(minX - upperLeft.getX()));
        upperLeft.setX(minX);
        
        double maxX = moreRect.getCorner().getX();
        dimensions.addX(-((upperLeft.getX() + dimensions.getX()) - maxX));
    }
    
    /*LOG(WIDGET, "scrollbar bar is " << WidgetRect(upperLeft, dimensions)
        << " out of " << bounds);*/
    
    widget->getBar()->updateLayout(WidgetRect(upperLeft, dimensions));
    
    WidgetPoint sliderCorner = upperLeft;
    WidgetPoint sliderDimensions = dimensions;
    
    if(widget->isVertical()) {
        sliderCorner.addY(dimensions.getY() * from);
        sliderDimensions.setY(dimensions.getY() * (to - from));
    }
    else {
        sliderCorner.addX(dimensions.getX() * from);
        sliderDimensions.setX(dimensions.getX() * (to - from));
    }
    
    widget->getSlider()->updateLayout(
        WidgetRect(sliderCorner, sliderDimensions));
    
    /*LOG(WIDGET, "scrollbar slider is "
        << WidgetRect(sliderCorner, sliderDimensions));*/
}

void ScrollbarLayout::update(const WidgetRect &newBounds) {
    this->bounds = newBounds;
    update();
}

}  // namespace Widget
}  // namespace Project
