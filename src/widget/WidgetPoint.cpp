#include <ostream>
#include "WidgetPoint.h"

namespace Project {
namespace Widget {

WidgetPoint WidgetPoint::plusXOf(const WidgetPoint &other) {
    return WidgetPoint(
        getX() + other.getX(),
        getY());
}

WidgetPoint WidgetPoint::plusYOf(const WidgetPoint &other) {
    return WidgetPoint(
        getX(),
        getY() + other.getY());
}

WidgetPoint WidgetPoint::operator + (const WidgetPoint &other) {
    return WidgetPoint(
        getX() + other.getX(),
        getY() + other.getY());
}

WidgetPoint::operator Math::Point () {
    return Math::Point(
        getX(),
        getY(),
        -1.0);
}

std::ostream &operator << (std::ostream &stream, const WidgetPoint &point) {
    stream << '(' << point.getX() << ", " << point.getY() << ')';
    return stream;
}

}  // namespace Widget
}  // namespace Project
