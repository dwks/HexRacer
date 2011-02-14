#include <ostream>
#include <istream>
#include <cmath>

#include "Point.h"
#include "Values.h"
//#include "Point2D.h"

namespace Project {
namespace Math {

Point Point::point2D(double u, double v, Axis project_axis) {

	switch (project_axis) {
		case X_AXIS:
			return Point(0.0f, u, v);
		case Y_AXIS:
			return Point(u, 0.0f, v);
		case Z_AXIS:
			return Point(u, v, 0.0f);
	}

	return Point(u, v, 0.0f);
}

Point Point::point2D(Point point, Axis project_axis) {
	point.setCoord(0.0f, project_axis);
	return point;
}

double Point::getU(Axis project_axis) const {
	switch (project_axis) {
		case X_AXIS:
			return getY();
		case Y_AXIS: case Z_AXIS:
			return getX();
	}
	return 0.0f;
}

double Point::getV(Axis project_axis) const {
	switch (project_axis) {
		case X_AXIS: case Y_AXIS:
			return getZ();
		 case Z_AXIS:
			return getY();
	}
	return 0.0f;
}

void Point::setU(double u, Axis project_axis) {
	switch (project_axis) {
		case X_AXIS:
			setY(u);
			return;
		case Y_AXIS: case Z_AXIS:
			setX(u);
			return;
	}
}

void Point::setV(double v, Axis project_axis) {
	switch (project_axis) {
		case X_AXIS: case Y_AXIS:
			setZ(v);
			return;
		 case Z_AXIS:
			setY(v);
			return;
	}
}

double Point::getCoord(Axis axis) const {
	switch (axis) {
		case X_AXIS:
			return getX();
		case Y_AXIS:
			return getY();
		case Z_AXIS:
			return getZ();
	}
	return 0.0f;
}

void Point::setCoord(double coord, Axis axis) {
	switch (axis) {
		case X_AXIS:
			setX(coord); return;
		case Y_AXIS:
			setY(coord); return;
		case Z_AXIS:
			setZ(coord); return;
	}
}


double Point::length() const {
    return std::sqrt(
        (getX() * getX()) +
        (getY() * getY()) +
        (getZ() * getZ()));
}

double Point::lengthSquared() const {
 return (getX() * getX()) +
        (getY() * getY()) +
        (getZ() * getZ());
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

double Point::distanceSquared(const Point &other) const {

    double x_diff = (getX() - other.getX());
    double y_diff = (getY() - other.getY());
    double z_diff = (getZ() - other.getZ());
    
    return (x_diff * x_diff
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
        (getX() * -s) + (getZ() * c),
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

Point Point::rotateAxis(Axis axis, double angle) const {

	switch (axis) {
		case X_AXIS:
			return rotateX(angle);
		case Y_AXIS:
			return rotateY(angle);
		case Z_AXIS:
			return rotateZ(angle);
	}

	return *this;
}

Point Point::rotate90CW(Axis axis) const {
	return point2D(getV(axis), -getU(axis), axis);
}

Point Point::rotate90CCW(Axis axis) const {
	return point2D(-getV(axis), getU(axis), axis);
}

double Point::dotProduct(const Point &other) const {
    return (getX() * other.getX())
        + (getY() * other.getY())
        + (getZ() * other.getZ());
}

double Point::unitDotProduct(const Point &other) const {
	return dotProduct(other)/(length()*other.length());
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

Point Point::operator - () const {
    return Point(
        -getX(),
        -getY(),
        -getZ(),
        getW());
}

Point Point::operator * (double factor) const {
    return Point(
        getX() * factor,
        getY() * factor,
        getZ() * factor,
        getW());
}

Point Point::coordinateMultiply(const Point &other) const {
	return Point(
        getX() * other.getX(),
        getY() * other.getY(),
        getZ() * other.getZ(),
        getW() * other.getW());
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

void Point::operator -= (const Point &other) {
    setX(getX() - other.getX());
    setY(getY() - other.getY());
    setZ(getZ() - other.getZ());
    setW(getW() - other.getW());
}

void Point::operator *= (double factor) {

    setX(getX()*factor);
    setY(getY()*factor);
    setZ(getZ()*factor);
    setW(getW()*factor);

}
void Point::operator /= (double factor) {

    setX(getX()/factor);
    setY(getY()/factor);
    setZ(getZ()/factor);
    setW(getW()/factor);
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

void Point::operator = (const Point &other) {
	setX(other.getX());
	setY(other.getY());
	setZ(other.getZ());
	setW(other.getW());
}

std::ostream &operator << (std::ostream &stream, const Point &point) {
    stream << '('
        << point.getX() << ','
        << point.getY() << ','
        << point.getZ() << ','
        << point.getW() << ')';
    
    return stream;
}

std::istream &operator >> (std::istream &stream, Point &point) {
    double x, y, z, w;
    char c;
    stream >> c >> x >> c >> y >> c >> z >> c >> w >> c;
    
    point = Math::Point(x, y, z, w);
    
    return stream;
}

Point operator * (double factor, const Point &point) {
    return point * factor;
}

}  // namespace Math
}  // namespace Project
