#include "FinishPlaneObject.h"
#include "MapEditorConstants.h"
#include "math/Geometry.h"

FinishPlaneObject::FinishPlaneObject(HRMap* _map)
	: map(_map) {

	Point plane_normal = _map->getFinishPlane().getNormal();

	Math::Point longitude_vector = Point::point2D(plane_normal, Y_AXIS);
	setRotation(Geometry::vectorTo2DAngle(longitude_vector, Y_AXIS)-(PI*0.5), YAW);
	setRotation(asin(plane_normal.getY()), PITCH);
}

BoundingBox3D FinishPlaneObject::getBoundingBox() const {
	return BoundingBox3D(MAP_EDITOR_FINISH_PLANE_BOX_SIZE,
		MAP_EDITOR_FINISH_PLANE_BOX_SIZE,
		MAP_EDITOR_FINISH_PLANE_BOX_SIZE, getPosition());
}

void FinishPlaneObject::setRotation(double radians, RotationAxis axis) {
	transform.setRotationRadians(radians, axis);
	map->getFinishPlane().setNormal(transform.getVector());
}
