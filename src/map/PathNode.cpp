#include "PathNode.h"
#include "misc/StdVectorFunctions.h"

namespace Project {
namespace Map {

	PathNode::PathNode(Math::Point position)
		: Math::Vertex3D(position) {
			progress = 0.0;
	}

	PathNode::PathNode(double x, double y, double z)
		: Math::Vertex3D(x, y, z) {
			progress = 0.0;
	}

	void PathNode::linkToNode(PathNode* node) {

		if (Misc::vectorRemoveOneElement(nextNodes, node)) {
			Misc::vectorRemoveOneElement(node->previousNodes, this);
		}
		else if (Misc::vectorRemoveOneElement(previousNodes, node)) {
			Misc::vectorRemoveOneElement(node->nextNodes, this);
		}
		else {
			nextNodes.push_back(node);
			node->previousNodes.push_back(this);
		}

	}

	void PathNode::disassociateNode(PathNode* node) {
		Misc::vectorRemoveOneElement(nextNodes, node);
		Misc::vectorRemoveOneElement(previousNodes, node);
	}


}  // namespace Map
}  // namespace Project
