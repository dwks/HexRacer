#include <ostream>
#include "WidgetPoint.h"

namespace Project {
namespace Widget {

WidgetPoint WidgetPoint::plusX(double x) {
    return WidgetPoint(
        getX() + x,
        getY());
}

WidgetPoint WidgetPoint::plusY(double y) {
    return WidgetPoint(
        getX(),
        getY() + y);
}

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

WidgetPoint WidgetPoint::operator + (const WidgetPoint &other) const {
    return WidgetPoint(
        getX() + other.getX(),
        getY() + other.getY());
}

WidgetPoint WidgetPoint::operator - (const WidgetPoint &other) const {
    return WidgetPoint(
        getX() - other.getX(),
        getY() - other.getY());
}

WidgetPoint &WidgetPoint::operator += (const WidgetPoint &other) {
    setX(getX() + other.getX());
    setY(getY() + other.getY());
    
    return *this;
}

WidgetPoint &WidgetPoint::operator -= (const WidgetPoint &other) {
    setX(getX() - other.getX());
    setY(getY() - other.getY());
    
    return *this;
}

WidgetPoint WidgetPoint::operator * (double factor) const {
    return WidgetPoint(
        factor * getX(),
        factor * getY());
}

WidgetPoint &WidgetPoint::operator *= (double factor) {
    setX(getX() * factor);
    setY(getY() * factor);
    
    return *this;
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
