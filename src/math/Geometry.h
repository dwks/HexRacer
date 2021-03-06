#ifndef PROJECT_MATH__GEOMETRY_H
#define PROJECT_MATH__GEOMETRY_H

#include "Point.h"
#include "Ray.h"
#include "RayIntersection.h"

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
    static void intersectLine(const Point& start, const Point& end, const Point& point,
        Point *closest, double *segmentAlpha = NULL);
    
    /** Calculates the closest point on a line segment to @a point.
        
        @return True if the line through the closest point and @a point is
            perpendicular to the given line segment.
    */
    static bool intersectLineSegment(const Point& start, const Point& end, const Point& point,
        Point *closest);
    
    /** Finds the closest point on a line segment to @a point, in 3D.
    */
    static Point intersectLine3D(const Point& start, const Point& end, const Point& point);
    
    /** Returns the Euclidean distance between two points.
    */
    //static double distance(const Point& one, const Point& two);
	
	/** Returns true iff the 2D line represents by points @a line_a and @a line_b
		intersects the horizontal 2D line at @a hline_v from @a hline_umin to
		@a hline_umax on the plane @a project_axis
	*/
	static bool lineHLineIntersects2D(const Point& line_a, const Point& line_b, double hline_v, double hline_umin, double hline_umax, Axis project_axis = Z_AXIS);
	
	/** Returns true iff the 2D line represents by points @a line_a and @a line_b
		intersects the vertical 2D line at @a hline_u from @a hline_vmin to
		@a hline_vmax on the plane @a project_axis
	*/
	static bool lineVLineIntersects2D(const Point& line_a, const Point& line_b, double hline_u, double vline_vmin, double vline_vmax, Axis project_axis = Z_AXIS);


	/** Determines if two points are on the same side of a line
        
        @return True if @a point_1 and @a point_2 are on the same side of
			the line denoted by @a line_a and @a line_b
    */
	static bool sameSideOfLine2D(const Point& line_a, const Point& line_b, const Point& point_1, const Point& point_2, Axis project_axis = Z_AXIS);

	/** Returns true iff the point @a is on the front-side of the plane represented
		by the point @a plane_point and normal @a plane_normal
	*/
	static bool frontOfPlane(const Point& plane_point, const Point& plane_normal, const Point& point);

	/** Computes the normal of a triangle represented by three points in counter-clockwise order
		as seen facing the front face
	*/
	static Point triangleNormal(const Point& tri_a, const Point& tri_b, const Point& tri_c);

	static RayIntersection rayPlaneIntersection(const Ray& ray, const Point& plane_point, const Point& plane_normal);

	static double vectorTo2DAngle(const Point& vector, Axis project_axis = Z_AXIS);

	static double getUOfLine(const Point& start, const Point& end, const Point& midpoint);
	
};

}  // namespace Math
}  // namespace Project

#endif
