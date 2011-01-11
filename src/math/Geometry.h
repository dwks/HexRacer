#ifndef PROJECT_MATH__GEOMETRY_H
#define PROJECT_MATH__GEOMETRY_H

#include "Point.h"
//#include "Point2D.h"

namespace Project {
namespace Math {

class Geometry {
public:
    /** Calculates the closest point on the line through [@a start, @a end] to
        @a point. Note: this function only works if start, end, and point all
        have the same Z value (i.e. it performs 2D intersections).
        
        @param closest The intersection point is stored in this variable if it
            is not NULL. Note that this point may not lie on the line segment
            strictly between @a start and @a end.
        @param segmentAlpha Set to the affine factor of the intersection point
            on the line segment. This may be useful for boundary detection: if
            this value is less than zero, the point lies on the far side of
            @a start, and similarly on the far side of @a end if it is larger
            than one.
    */
    static void intersectLine(Point start, Point end, Point point,
        Point *closest, double *segmentAlpha = NULL);
    
    /** Calculates the closest point on a line segment to @a point.
        
        @return True if the line through the closest point and @a point is
            perpendicular to the given line segment.
    */
    static bool intersectLineSegment(Point start, Point end, Point point,
        Point *closest);
    
    /** Returns the Euclidean distance between two points.
    */
    static double distance(Point one, Point two);

	/*
	static bool lineHLineIntersects2D(Point2D line_a, Point2D line_b, float hline_y, float hline_xmin, float hline_xmax);
	static bool lineVLineIntersects2D(Point2D line_a, Point2D line_b, float hline_x, float vline_ymin, float vline_ymax);
	static bool triangleBoxIntersects2D(Point2D tri_a, Point2D tri_b, Point2D tri_c, Point2D box1, Point2D box2);
	*/
	//static bool lineSegementIntersection2D(Point2D line1a, Point2D line1b, Point2D line2a, Point2D line2b, Point2D *intersection = NULL);
};

}  // namespace Math
}  // namespace Project

#endif
