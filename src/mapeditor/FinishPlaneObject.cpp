#include "FinishPlaneObject.h"
#include "MapEditorConstants.h"

FinishPlaneObject::FinishPlaneObject(HRMap* _map)
	: map(_map) {
	setRotation(0.0, YAW);
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
