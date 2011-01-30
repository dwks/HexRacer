#include "PathNode.h"

namespace Project {
namespace Map {

	PathNode::PathNode(Math::Point position)
		: Math::Vertex3D(position) {
	}

	PathNode::PathNode(double x, double y, double z)
		: Math::Vertex3D(x, y, z) {
	}

}  // namespace Map
}  // namespace Project
