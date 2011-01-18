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


bool Geometry::lineHLineIntersects2D(Point line_a, Point line_b, 
									 double hline_v, double hline_umin,
									 double hline_umax, Axis project_axis)
{

	//Swap the min and max values if they are not ordered correctly
	if (hline_umin > hline_umax) {
		double temp = hline_umin;
		hline_umin = hline_umax;
		hline_umax = temp;
	}

	//Get the u and v values of the input line points
	double line_a_u = line_a.getU(project_axis);
	double line_a_v = line_a.getV(project_axis);
	double line_b_u = line_b.getU(project_axis);
	double line_b_v = line_b.getV(project_axis);

	//Get the min and max u values of the input line
	double l_min_u = minimum(line_a_u, line_b_u);
	double l_max_u = maximum(line_a_u, line_b_u);

	//Check for a horizontal input line (parallel lines)
	if (line_a_v == line_b_v) {
		//Check for overlap between the range and match of v value
		return (line_a_v == hline_v && l_min_u <= hline_umax && l_max_u >= hline_umin);
	}

	//Get the u value of the intersection point
	double intersect_u = line_a_u+((hline_v-line_a_v)/(line_b_v-line_a_v))*(line_b_u-line_a_u);
	//Check if the u value is within the accepted ranges
	return (intersect_u >= hline_umin && intersect_u <= hline_umax
		&& intersect_u >= l_min_u && intersect_u <= l_max_u);
	
}

bool Geometry::lineVLineIntersects2D(Point line_a, Point line_b, 
									 double vline_u, double vline_vmin,
									 double vline_vmax, Axis project_axis)
{
	//Swap the min and max values if they are not ordered correctly
	if (vline_vmin > vline_vmax) {
		double temp = vline_vmin;
		vline_vmin = vline_vmax;
		vline_vmax = temp;
	}

	//Get the u and v values of the input line points
	double line_a_u = line_a.getU(project_axis);
	double line_a_v = line_a.getV(project_axis);
	double line_b_u = line_b.getU(project_axis);
	double line_b_v = line_b.getV(project_axis);

	//Get the min and max v values of the input line
	double l_min_v = minimum(line_a_v, line_b_v);
	double l_max_v = maximum(line_a_v, line_b_v);

	//Check for a vertical input line (parallel lines)
	if (line_a_u == line_b_u) {
		//Check for overlap between the range and match of v value
		return (line_a_u == vline_u && l_min_v <= vline_vmax && l_max_v >= vline_vmin);
	}

	//Get the v value of the intersection point
	double intersect_v = line_a_v+((vline_u-line_a_u)/(line_b_u-line_a_u))*(line_b_v-line_a_v);
	//Check if the v value is within the accepted ranges
	return (intersect_v >= vline_vmin && intersect_v <= vline_vmax
		&& intersect_v >= l_min_v && intersect_v <= l_max_v);
	
}

bool Geometry::sameSideOfLine2D(Point line_a, Point line_b, Point point_1, Point point_2, Axis project_axis) {

	line_a = Point::point2D(line_a, project_axis);
	line_b = Point::point2D(line_b, project_axis);
	point_1 = Point::point2D(point_1, project_axis);
	point_2 = Point::point2D(point_2, project_axis);

	Point line_normal = (line_b-line_a).rotate90CW(project_axis);

	return (frontOfPlane(line_a, line_normal, point_1) == frontOfPlane(line_a, line_normal, point_2));

	/*
	Point cp1 = (line_b - line_a).crossProduct(point_1 - line_a);
    Point cp2 = (line_b - line_a).crossProduct(point_2 - line_a);
	return (cp1.dotProduct(cp2) >= 0.0f);
	*/

}

bool Geometry::frontOfPlane(Point plane_point, Point plane_normal, Point point) {
	return ((point-plane_point).dotProduct(plane_normal) >= 0.0f);
}

Point Geometry::triangleNormal(Point tri_a, Point tri_b, Point tri_c) {
	Point u = tri_b-tri_a;
	Point v = tri_c-tri_a;
	Point normal = u.crossProduct(v);
	normal.normalize();
	return normal;
}

}  // namespace Math
}  // namespace Project
