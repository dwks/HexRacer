#include "BoundingTriangle2D.h"
#include "BoundingBox2D.h"
#include "Values.h"
#include "Geometry.h"

#include "log/Logger.h"

namespace Project {
namespace Math {

	BoundingTriangle2D::BoundingTriangle2D(Point vertex1, Point vertex2, Point vertex3, Axis project_axis)
		: BoundingObject2D(project_axis)
	{
		vertex[0] = Point::point2D(vertex1, project_axis);
		vertex[1] = Point::point2D(vertex2, project_axis);
		vertex[2] = Point::point2D(vertex3, project_axis);
	}

	double BoundingTriangle2D::minX() const {
		return minimum(minimum(getVertex(0).getX(), getVertex(1).getX()), getVertex(2).getX());
	}
	double BoundingTriangle2D::minY() const {
		return minimum(minimum(getVertex(0).getY(), getVertex(1).getY()), getVertex(2).getY());
	}
	double BoundingTriangle2D::minZ() const {
		return minimum(minimum(getVertex(0).getZ(), getVertex(1).getZ()), getVertex(2).getZ());
	}
	double BoundingTriangle2D::maxX() const {
		return maximum(maximum(getVertex(0).getX(), getVertex(1).getX()), getVertex(2).getX());
	}
	double BoundingTriangle2D::maxY() const {
		return maximum(maximum(getVertex(0).getY(), getVertex(1).getY()), getVertex(2).getY());
	}
	double BoundingTriangle2D::maxZ() const {
		return maximum(maximum(getVertex(0).getZ(), getVertex(1).getZ()), getVertex(2).getZ());
	}

	Point BoundingTriangle2D::centroid() const {
		return ( getVertex(0) + getVertex(1) + getVertex(2) )/3.0f;
	}

	bool BoundingTriangle2D::isInside2D(const BoundingObject2D& bounding_obj) const {

		if (bounding_obj.getProjectAxis() != projectAxis)
			return false;

		return (
			bounding_obj.pointInside(getVertex(0)) &&
			bounding_obj.pointInside(getVertex(1)) &&
			bounding_obj.pointInside(getVertex(2))
			);
	}

	bool BoundingTriangle2D::pointInside(const Point& p) const {
        // all of these actually work, it's just a question of speed.
#if 1
		return (Geometry::sameSideOfLine2D(getVertex(0), getVertex(1), getVertex(2), p, projectAxis) &&
			Geometry::sameSideOfLine2D(getVertex(1), getVertex(2), getVertex(0), p, projectAxis) &&
			Geometry::sameSideOfLine2D(getVertex(2), getVertex(0), getVertex(1), p, projectAxis));
#elif 1
        Point p2D = Point::point2D(p, projectAxis);
        
        Point v1 = getVertex(0) - getVertex(1);
        Point v2 = getVertex(1) - getVertex(2);
        Point v3 = getVertex(2) - getVertex(0);

		Point w1 = p2D - getVertex(1);
		Point w2 = p2D - getVertex(2);
		Point w3 = p2D - getVertex(0);
        
        Point p1 = w1.crossProduct(v1);
        Point p2 = w2.crossProduct(v2);
        Point p3 = w3.crossProduct(v3);
        
        if(p2.dotProduct(p1) >= 0 && p2.dotProduct(p3) >= 0) return true;
        
        return false;
#else
        Point p2D;
        Point projectedVertex[3];
        
        switch(projectAxis) {
        case X_AXIS:
            p2D = Point(p.getY(), p.getZ());
            projectedVertex[0] = Point(getVertex(0).getY(), getVertex(0).getZ());
            projectedVertex[1] = Point(getVertex(1).getY(), getVertex(1).getZ());
            projectedVertex[2] = Point(getVertex(2).getY(), getVertex(2).getZ());
            break;
        case Y_AXIS:
            p2D = Point(p.getX(), p.getZ());
            projectedVertex[0] = Point(getVertex(0).getX(), getVertex(0).getZ());
            projectedVertex[1] = Point(getVertex(1).getX(), getVertex(1).getZ());
            projectedVertex[2] = Point(getVertex(2).getX(), getVertex(2).getZ());
            break;
        case Z_AXIS:
            p2D = Point::point2D(p, Z_AXIS);
            projectedVertex[0] = getVertex(0);
            projectedVertex[1] = getVertex(1);
            projectedVertex[2] = getVertex(2);
            break;
        }
        
        bool odd = false;
        
        static const int VERTICES = 3;
        int last = VERTICES - 1;
        for(int x = 0; x < VERTICES; x ++) {
            const Point &cur = projectedVertex[x];
            const Point &prev = projectedVertex[last];
            
            bool x_y = cur.getY() < p2D.getY();
            bool last_y = prev.getY() < p2D.getY();
            
            if(x_y != last_y) {
                if(cur.getX()
                    + (p2D.getY() - cur.getY()) / (prev.getY() - cur.getY())
                        * (p2D.getX() - cur.getX()) < p2D.getX()) {
                    
                    odd = !odd;
                }
            }
            
            last = x;
        }
        
        if(odd != (Geometry::sameSideOfLine2D(getVertex(0), getVertex(1), getVertex(2), p, projectAxis) &&
            Geometry::sameSideOfLine2D(getVertex(1), getVertex(2), getVertex(0), p, projectAxis) &&
            Geometry::sameSideOfLine2D(getVertex(2), getVertex(0), getVertex(1), p, projectAxis))) {
            
            LOG(OPENGL, p2D << " inside " << projectedVertex[0] << "," << projectedVertex[1] << ","
                << projectedVertex[2]);
            LOG(OPENGL, "odd is " << odd << " which is incorrect");
        }
        
        return odd;
#endif
	}

	bool BoundingTriangle2D::intersects2D(const BoundingObject2D& bounding_obj) const {

		if (bounding_obj.getProjectAxis() != projectAxis)
			return false;

		switch (bounding_obj.getObjectType()) {

			case TRIANGLE:
				return false;

			case BOX: {
				const BoundingBox2D* box_2D = (const BoundingBox2D*) &bounding_obj;
				//2D Triangle-Box Intersection

				//Check if any of the box's vertices is inside this triangle
				for (short i = 0; i < 4; i++) {
					if (pointInside(box_2D->getCorner(i))) {
						return true;
					}
				}
				//Check if any of this triangle's vertices is inside the box
				for (short i = 0; i < 3; i++) {
					if (box_2D->pointInside(getVertex(i))) {
						return true;
					}
				}
				//Check if any of this triangle's edges intersects any of the box's edges
				for (short tri_edge = 0; tri_edge < 3; tri_edge++) {
					Point line_a = getVertex(tri_edge);
					Point line_b = getVertex((tri_edge+1) % 3);
					
					for (short box_edge = 0; box_edge < 4; box_edge++) {
						switch (box_edge) {
							case 0:
								//Min horizonal edge
								if (Geometry::lineHLineIntersects2D(line_a, line_b,
									box_2D->minV(), box_2D->minU(), box_2D->maxU(), projectAxis))
									return true;
								;break;
							case 1:
								//Max horizonal edge
								if (Geometry::lineHLineIntersects2D(line_a, line_b,
									box_2D->maxV(), box_2D->minU(), box_2D->maxU(), projectAxis))
									return true;
								;break;
							case 2:
								//Min vertical edge
								if (Geometry::lineVLineIntersects2D(line_a, line_b,
									box_2D->minU(), box_2D->minV(), box_2D->maxV(), projectAxis))
									return true;
								;break;
							case 3:
								//Max vertical edge
								if (Geometry::lineVLineIntersects2D(line_a, line_b,
									box_2D->maxU(), box_2D->minV(), box_2D->maxV(), projectAxis))
									return true;
								;break;
						}
					}

				}

				//Intersection test failed
				return false;
            }
            
            default:
                break;

		}

		//Defer to the other object's interesection tests
		return bounding_obj.intersects2D(*this);
	}

	void BoundingTriangle2D::translate(const Point& translation) {
		for (int i = 0; i < 3; i++) {
			vertex[i] += translation;
		}
	}

	Point BoundingTriangle2D::getVertex(short vert_index) const {
		return vertex[vert_index];
	}

}  // namespace Math
}  // namespace Project
