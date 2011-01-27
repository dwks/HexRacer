#include "PathNodeObject.h"
#include "MapEditorConstants.h"

BoundingBox3D PathNodeObject::getBoundingBox() const {
	return BoundingBox3D(MAP_EDITOR_PATHNODE_LENGTH*2.0,
		MAP_EDITOR_PATHNODE_HEIGHT*2.0,
		MAP_EDITOR_PATHNODE_LENGTH*2.0, position);
}