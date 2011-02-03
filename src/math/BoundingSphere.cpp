#include "BoundingSphere.h"
#include "BoundingPlane3D.h"
#include "BoundingConvexHull3D.h"
#include "Vertex2D.h"
#include <math.h>
#include "Values.h"

namespace Project {
namespace Math {

	BoundingSphere::BoundingSphere(Point _position, double _radius) {
		position = _position;
		setRadius(_radius);
	}

	void BoundingSphere::setRadius(double _radius) {
		radius = std::abs(_radius);
		radiusSquared = radius * radius;
	}

	bool BoundingSphere::isInside(const BoundingObject& bounding_obj) const {

		if (!bounding_obj.is2D()) {

			const BoundingObject3D& bound_3D = (BoundingObject3D&) bounding_obj;

			switch (bound_3D.getObjectType()) {

				case BOX:
					return (
						bound_3D.minX() <= minX() && bound_3D.maxX() >= maxX() &&
						bound_3D.minY() <= minY() && bound_3D.maxY() >= maxY() &&
						bound_3D.minZ() <= minZ() && bound_3D.maxZ() >= maxZ()
						);

				case CIRCLE: {
					const BoundingSphere& sphere_3D = (const BoundingSphere&) (bound_3D);
					return (
						(sphere_3D.centroid().distance(position)+radius) <= sphere_3D.getRadius()
						);
				}

				case PLANE: {
					const BoundingPlane3D& plane_3D = (const BoundingPlane3D&) (bound_3D);
					return plane_3D.pointInside(position-plane_3D.getNormal()*radius);
				}

				case CONVEX_HULL: {
					const std::vector<BoundingPlane3D>& planes = ((const BoundingConvexHull3D&) bound_3D).getPlanes();
					for (unsigned int i = 0; i < planes.size(); i++) {
						if (!isInside(planes[i])) {
							return false;
						}
					}
					return (planes.size() > 0);
				}


				default: break;
			}

		}

		return false;
	}

	BoundingObject2D* BoundingSphere::projectTo2D(Axis project_axis) const {
		return new Vertex2D();
	}

	bool BoundingSphere::pointInside(const Point& p) const {
		return (position.distanceSquared(p) <= radiusSquared);
	}

	bool BoundingSphere::intersects3D(const BoundingObject3D& bounding_obj) const {

		

		switch (bounding_obj.getObjectType()) {

			case VERTEX:
				return pointInside(bounding_obj.centroid());

			case BOX: {
				Point p;
				p.setX( minimum ( 
					maximum( position.getX(), bounding_obj.minX() ),
					bounding_obj.maxX() )
					);
				p.setY( minimum ( 
					maximum( position.getY(), bounding_obj.minY() ),
					bounding_obj.maxY() )
					);
				p.setZ( minimum ( 
					maximum( position.getZ(), bounding_obj.minZ() ),
					bounding_obj.maxZ() )
					);
				return pointInside(p);
			}

			case CIRCLE: {
				const BoundingSphere*sphere_3D = (const BoundingSphere*) (&bounding_obj);
				return (
					(sphere_3D->centroid().distance(position)-radius) <= sphere_3D->getRadius()
					);
			}

			case PLANE: {
				const BoundingPlane3D* plane_3D = (const BoundingPlane3D*) (&bounding_obj);
				return plane_3D->pointInside(position+plane_3D->getNormal()*radius);
			}

			case CONVEX_HULL: {
				const BoundingConvexHull3D* ch_3D = (const BoundingConvexHull3D*) &bounding_obj;
				const std::vector<BoundingPlane3D>& planes = ch_3D->getPlanes();
				for (unsigned int i = 0; i < planes.size(); i++) {
					if (!intersects(planes[i])) {
						return false;
					}
				}
				return (planes.size() > 0);
			}

			default: break;
			
		}
		
		return false;
	}

	void BoundingSphere::expandToInclude(const Point& p) {
		double distSquared = position.distanceSquared(p);
		if (distSquared > radiusSquared) {
			setRadius(std::sqrt(distSquared));
		}
	}

}  // namespace Math
}  // namespace Project
