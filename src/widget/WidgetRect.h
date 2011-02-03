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
    
    WidgetPoint getCorner() const { return corner; }
    WidgetPoint getDimensions() const { return dimensions; }
};

std::ostream &operator << (std::ostream &stream, const WidgetRect &rect);

}  // namespace Widget
}  // namespace Project

#endif
