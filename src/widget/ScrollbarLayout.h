#ifndef PROJECT_WIDGET__SCROLLBAR_LAYOUT_H
#define PROJECT_WIDGET__SCROLLBAR_LAYOUT_H

#include "Layout.h"

namespace Project {
namespace Widget {

class ScrollbarWidget;

class ScrollbarLayout : public Layout {
private:
    ScrollbarWidget *widget;
    double from, to;
    WidgetRect bounds;
public:
    ScrollbarLayout(ScrollbarWidget *widget, const WidgetRect &bounds)
        : widget(widget), from(0.0), to(0.0), bounds(bounds) {}
    
    void setViewport(double from, double to);
    
    virtual void update();
    virtual void update(const WidgetRect &newBounds);
    
    virtual WidgetRect getBoundingRect() const { return bounds; }
};

}  // namespace Widget
}  // namespace Project

#endif
