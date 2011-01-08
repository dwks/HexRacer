#include <ostream>
#include <cmath>

#include "Point.h"
#include "Values.h"

namespace Project {
namespace Math {

double Point::length() const {
    return std::sqrt(
        (getX() * getX()) +
        (getY() * getY()) +
        (getZ() * getZ()));
}

double Point::distance(const Point &other) const {
    double x_diff = (getX() - other.getX());
    double y_diff = (getY() - other.getY());
    double z_diff = (getZ() - other.getZ());
    
    return std::sqrt(
        x_diff * x_diff
        + y_diff * y_diff
        + z_diff * z_diff);
}

Point Point::normalized() const {
    double the_length = length();
    
    return Point(
        getX() / the_length,
        getY() / the_length,
        getZ() / the_length,
        0.0);
}

void Point::normalize() {
    double the_length = length();
    
    setX(getX() / the_length);
    setY(getY() / the_length);
    setZ(getZ() / the_length);
    setW(0.0);
}

void Point::restrictTo(Math::Point restriction) {
    if(std::fabs(getX()) > restriction.getX()) {
        setX(sign(getX()) * restriction.getX());
    }
    if(std::fabs(getY()) > restriction.getY()) {
        setY(sign(getY()) * restriction.getY());
    }
    if(std::fabs(getZ()) > restriction.getZ()) {
        setZ(sign(getZ()) * restriction.getZ());
    }
    
    setW(1.0);
}

Point Point::rotateX(double angle) const {
    double s = std::sin(angle);
    double c = std::cos(angle);
    
    return Point(
        getX(),
        (getY() * c) + (getZ() * -s),
        (getY() * s) + (getZ() * c),
        getW());
}

Point Point::rotateY(double angle) const {
    double s = std::sin(angle);
    double c = std::cos(angle);
    
    return Point(
        (getX() * c) + (getZ() * s),
        getY(),
        (getX() * -s) + (getZ() * s),
        getW());
}

Point Point::rotateZ(double angle) const {
    double s = std::sin(angle);
    double c = std::cos(angle);
    
    return Point(
        (getX() * c) + (getY() * -s),
        (getX() * s) + (getY() * c),
        getZ(),
        getW());
}

double Point::dotProduct(const Point &other) const {
    return (getX() * other.getX())
        + (getY() * other.getY())
        + (getZ() * other.getZ());
}

Point Point::crossProduct(const Point &other) const {
    return Point(
        (getY() * other.getZ()) - (getZ() * other.getY()),
        (getZ() * other.getX()) - (getX() * other.getZ()),
        (getX() * other.getY()) - (getY() * other.getX()),
        getW());
}

Point Point::operator + (const Point &other) const {
    return Point(
        getX() + other.getX(),
        getY() + other.getY(),
        getZ() + other.getZ(),
        getW() + other.getW());
}

Point Point::operator - (const Point &other) const {
    return Point(
        getX() - other.getX(),
        getY() - other.getY(),
        getZ() - other.getZ(),
        getW() - other.getW());
}

Point Point::operator * (double factor) const {
    return Point(
        getX() * factor,
        getY() * factor,
        getZ() * factor,
        getW());
}

Point Point::operator / (double factor) const {
    return Point(
        getX() / factor,
        getY() / factor,
        getZ() / factor,
        getW());
}

Point Point::projectOnto(const Point &onto) const {
    return dotProduct(onto) * onto;
}

void Point::operator += (const Point &other) {
    setX(getX() + other.getX());
    setY(getY() + other.getY());
    setZ(getZ() + other.getZ());
    setW(getW() + other.getW());
}

bool Point::operator == (const Point &other) {
    return getX() == other.getX()
        && getY() == other.getY()
        && getZ() == other.getZ()
        && getW() == other.getW();
}

double Point::operator [] (int index) const {
    switch(index) {
    case 0:
        return getX();
    case 1:
        return getY();
    case 2:
        return getZ();
    case 3:
        return getW();
    default:
        return -1.0;
    }
}

double &Point::operator [] (int index) {
    switch(index) {
    case 0:
        return xp;
    case 1:
        return yp;
    case 2:
        return zp;
    default:
        return wp;
    }
}

std::ostream &operator << (std::ostream &stream, const Point &point) {
    stream << '('
        << point.getX() << ','
        << point.getY() << ','
        << point.getZ() << ','
        << point.getW() << ')';
    
    return stream;
}

Point operator * (double factor, const Point &point) {
    return point * factor;
}

}  // namespace Math
}  // namespace Project
