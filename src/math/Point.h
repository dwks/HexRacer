#ifndef PROJECT_MATH__POINT_H
#define PROJECT_MATH__POINT_H

#include <iosfwd>

namespace Project {
namespace Math {

/** Represents a four-dimensional affine point.
*/
class Point {
private:
    double xp, yp, zp, wp;
public:
    Point() : xp(0.0), yp(0.0), zp(0.0), wp(0.0) {}
    Point(double x, double y, double z = 0.0, double w = 1.0)
        : xp(x), yp(y), zp(z), wp(w) {}
    
    double getX() const { return xp; }
    double getY() const { return yp; }
    double getZ() const { return zp; }
    double getW() const { return wp; }
    
    void setX(double x) { xp = x; }
    void setY(double y) { yp = y; }
    void setZ(double z) { zp = z; }
    void setW(double w) { wp = w; }
    
    /** Returns the length of this Point (as if it were a vector).
    */
    double length() const;
    
    /** Returns the euclidean distance between this Point and @a other.
    */
    double distance(const Point &other) const;
    
    /** Returns a normalized vector (a vector of length 1) with the same
        direction as this one.
    */
    Point normalized() const;
    
    /** Normalizes this vector, setting it to a vector of length 1 with the
        same direction as this had originally.
    */
    void normalize();
    
    /** Applies a cap to restrict this Point such that the absolute values of
        the components are not greater than those in @a restriction.
    */
    void restrictTo(Math::Point restriction);
    
    /** Rotates this Point about the X axis.
        
        @param angle The angle to rotate through, specified in radians.
    */
    Point rotateX(double angle) const;
    
    /** Rotates this Point about the Y axis.
        
        @param angle The angle to rotate through, specified in radians.
    */
    Point rotateY(double angle) const;
    
    /** Rotates this Point about the Z axis.
        
        @param angle The angle to rotate through, specified in radians.
    */
    Point rotateZ(double angle) const;
    
    /** Returns the dot product of this vector with @a other.
    */
    double dotProduct(const Point &other) const;
    
    /** Returns the cross product of this vector with @a other.
    */
    Point crossProduct(const Point &other) const;
    
    Point operator + (const Point &other) const;
    Point operator - (const Point &other) const;
    
    Point operator * (double factor) const;
    
    /**
        Note: no divide-by-zero checking is performed here.
    */
    Point operator / (double factor) const;
    
    Point projectOnto(const Point &onto) const;
    
    void operator += (const Point &other);
    
    /** Compares two Points with exact floating-point comparisons.
    */
    bool operator == (const Point &other);
    
    double operator [] (int index) const;
    double &operator [] (int index);
};

std::ostream &operator << (std::ostream &stream, const Point &point);
std::istream &operator >> (std::istream &stream, Point &point);

Point operator * (double factor, const Point &point);

}  // namespace Math
}  // namespace Project

#endif
