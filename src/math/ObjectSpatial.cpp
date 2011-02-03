#include "ObjectSpatial.h"

namespace Project {
namespace Math {

double ObjectSpatial::minCoord(Axis axis) const {
	switch (axis) {
		case X_AXIS:
			return minX();
		case Y_AXIS:
			return minY();
		case Z_AXIS:
			return minZ();
	}
	return 0.0f;
}

double ObjectSpatial::maxCoord(Axis axis) const {
	switch (axis) {
		case X_AXIS:
			return maxX();
		case Y_AXIS:
			return maxY();
		case Z_AXIS:
			return maxZ();
	}
	return 0.0f;
}

double ObjectSpatial::widthCoord(Axis axis) const {
	switch (axis) {
		case X_AXIS:
			return widthX();
		case Y_AXIS:
			return widthY();
		case Z_AXIS:
			return widthZ();
	}
	return 0.0f;
}

ObjectSpatial::IntersectionType ObjectSpatial::intersectionType(const BoundingObject& bounding_obj) const {
	if (intersects(bounding_obj)) {
		if (isInside(bounding_obj))
			return INTERSECT_INSIDE;
		else
			return INTERSECT_INTERSECTS;
	}
	return INTERSECT_NONE;
}

}  // namespace Math
}  // namespace Project