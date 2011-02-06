#ifndef PROJECT_WIDGET__WIDGET_POINT_H
#define PROJECT_WIDGET__WIDGET_POINT_H

#include <iosfwd>

#include "math/Point.h"

namespace Project {
namespace Widget {

/** Represents a widget point.
    
    At the top level, the screen has size 1.0 by 1.0 in this coordinate space.
*/
class WidgetPoint {
private:
    double xp, yp;
public:
    WidgetPoint() : xp(0.0), yp(0.0) {}
    WidgetPoint(double xp, double yp) : xp(xp), yp(yp) {}
    
    double getX() const { return xp; }
    double getY() const { return yp; }
    
    void setX(double x) { xp = x; }
    void setY(double y) { yp = y; }
    
    WidgetPoint plusX(double x);
    WidgetPoint plusY(double y);
    WidgetPoint plusXOf(const WidgetPoint &other);
    WidgetPoint plusYOf(const WidgetPoint &other);
    
    void addX(double x) { xp += x; }
    void addY(double y) { yp += y; }
    
    WidgetPoint operator + (const WidgetPoint &other) const;
    WidgetPoint operator - (const WidgetPoint &other) const;
    WidgetPoint &operator += (const WidgetPoint &other);
    WidgetPoint &operator -= (const WidgetPoint &other);
    WidgetPoint operator * (double factor) const;
    WidgetPoint &operator *= (double factor);
    
    operator Math::Point ();
};

std::ostream &operator << (std::ostream &stream, const WidgetPoint &point);

}  // namespace Widget
}  // namespace Project

#endif
