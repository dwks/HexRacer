#include "PathNodeObject.h"
#include "MapEditorConstants.h"
#include "misc/StdVectorFunctions.h"
using namespace Misc;

BoundingBox3D PathNodeObject::getBoundingBox() const {
	return BoundingBox3D(MAP_EDITOR_PATHNODE_LENGTH*2.0,
		MAP_EDITOR_PATHNODE_HEIGHT*2.0,
		MAP_EDITOR_PATHNODE_LENGTH*2.0, getPosition());
}

void PathNodeObject::linkNode(PathNodeObject* node_object) {

	if (node_object) {
		PathNode* link_node = node_object->getNode();
		if (link_node && link_node != node) {
			//If the two nodes are already linked, remove the link, otherwise link them
			if (!vectorRemoveOneElement(node->getNextNodes(), link_node)
				&& !vectorRemoveOneElement(link_node->getNextNodes(), node)) {
				node->getNextNodes().push_back(link_node);
			}
		}
	}

}