#include "LightObject.h"
#include "MapEditorConstants.h"

BoundingBox3D LightObject::getBoundingBox() const {
	double size = MAP_EDITOR_LIGHT_SPHERE_RADIUS*2.0;
	return BoundingBox3D(size, size, size, light->getPosition());
}
