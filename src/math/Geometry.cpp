#include "Geometry.h"
#include "Values.h"

namespace Project {
namespace Math {

/** Rough draft of derivation:
    
        p + a*pv = s + b*v
        a*pv - b*v = s - p
        b*v + a*(-pv) = ps    (ps = p - s)
        
        b*v_x - a*pv_x = ps_x
        b*v_y - a*pv_y = ps_y
            =>  b*v_y = ps_y + a*pv_y
            =>  b = (ps_y + a*pv_y) / v_y
        
        ((ps_y + a*pv_y) / v_y)*v_x - a*pv_x = ps_x
        ((ps_y + a*pv_y) / v_y)*v_x = ps_x + a*pv_x
        (ps_y + a*pv_y) / v_y = (ps_x + a*pv_x) / v_x
        ps_y/v_y + a*pv_y/v_y = ps_x/v_x + a*pv_x/v_x
        a*pv_y/v_y - a*pv_x/v_x = ps_x/v_x - ps_y/v_y
        a*(pv_y/v_y - pv_x/v_x) = ps_x/v_x - ps_y/v_y
        
        a = (ps_x/v_x - ps_y/v_y) / (pv_y/v_y - pv_x/v_x)
        b = (ps_y + a*pv_y) / v_y
*/
void Geometry::intersectLine(Point start, Point end, Point point,
    Point *closest, double *segmentAlpha) {
    
    Point v = end - start;
    Point ps = point - start;
    Point pv = v.rotateZ(PI / 2.0);
    
    double a = (ps.getX()/v.getX() - ps.getY()/v.getY())
        / (pv.getY()/v.getY() - pv.getX()/v.getX());
    
    if(closest) {
        *closest = point + a * pv;
    }
    
    if(segmentAlpha) {
        double b = (ps.getY() + a*pv.getY()) / v.getY();
        *segmentAlpha = b;
    }
}

bool Geometry::intersectLineSegment(Point start, Point end, Point point,
    Point *closest) {
    
    Point intersection;
    double alpha;
    
    intersectLine(start, end, point, &intersection, &alpha);
    
    bool perpendicular = true;
    if(alpha < 0.0) {
        intersection = start;
        perpendicular = false;
    }
    else if(alpha > 1.0) {
        intersection = end;
        perpendicular = false;
    }
    
    if(closest) *closest = intersection;
    
    return perpendicular;
}

double Geometry::distance(Point one, Point two) {
    return (two - one).length();
}

}  // namespace Math
}  // namespace Project
