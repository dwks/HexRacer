#include "StartPointObject.h"
#include "MapEditorConstants.h"

BoundingBox3D StartPointObject::getBoundingBox() const {
	return BoundingBox3D(MAP_EDITOR_STARTPOINT_LENGTH*2.0,
		MAP_EDITOR_STARTPOINT_HEIGHT*2.0,
		MAP_EDITOR_STARTPOINT_LENGTH*2.0, getPosition());
}