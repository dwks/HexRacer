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
	std::vector<PathNode*> previousNodes;
	float progress;

public:
	PathNode(Math::Point position = Math::Point());
	PathNode(double x, double y, double z);

	int index;

	void linkToNode(PathNode* node);
	void disassociateNode(PathNode* node);

	const std::vector<PathNode*>& getNextNodes() const { return nextNodes; }
	const std::vector<PathNode*>& getPreviousNodes() const { return previousNodes; }

	float getProgress() const { return progress; }
	void setProgress(float _progress) { progress = _progress; }
};

}  // namespace Map
}  // namespace Project

#endif
