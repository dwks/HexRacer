#ifndef PROJECT_WIDGET__WIDGET_RECT_H
#define PROJECT_WIDGET__WIDGET_RECT_H

#include <iosfwd>

#include "WidgetPoint.h"

namespace Project {
namespace Widget {

class WidgetRect {
private:
    WidgetPoint corner, dimensions;
public:
    WidgetRect() {}
    WidgetRect(const WidgetPoint &corner, const WidgetPoint &dimensions)
        : corner(corner), dimensions(dimensions) {}
    WidgetRect(double x, double y, double w, double h)
        : corner(x, y), dimensions(w, h) {}
    
    const WidgetPoint &getCorner() const { return corner; }
    WidgetPoint &getCorner() { return corner; }
    
    const WidgetPoint &getDimensions() const { return dimensions; }
    WidgetPoint &getDimensions() { return dimensions; }
    
    double getWidth() const { return dimensions.getX(); }
    double getHeight() const { return dimensions.getY(); }
    
    bool pointInside(const WidgetPoint &point) const;
};

std::ostream &operator << (std::ostream &stream, const WidgetRect &rect);

}  // namespace Widget
}  // namespace Project

#endif
