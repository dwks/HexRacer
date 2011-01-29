#ifndef PROJECT_MAP__PATH_NODE_H
#define PROJECT_MAP__PATH_NODE_H

#include "math/Vertex3D.h"
#include <vector>

namespace Project {
namespace Map {

class PathNode
	: public Math::Vertex3D {
private:
	std::vector<PathNode*> nextNodes;
public:
	PathNode(Math::Point position = Math::Point())
		: Math::Vertex3D(position) {}
	PathNode(double x, double y, double z)
		: Math::Vertex3D(x, y, z) {}

	std::vector<PathNode*>& getNextNodes() { return nextNodes; }
	const std::vector<PathNode*>& getNextNodes() const { return nextNodes; }
};

}  // namespace Map
}  // namespace Project

#endif
