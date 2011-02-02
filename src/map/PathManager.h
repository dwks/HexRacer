#ifndef PROJECT_MAP__PATH_MANAGER_H
#define PROJECT_MAP__PATH_MANAGER_H

#include "PathNode.h"
#include "math/BSPTree3D.h"
#include "config.h"

namespace Project {
namespace Map {

class PathManager {
private:
	Math::BSPTree3D* pathTree;
public:

	PathManager(const vector<PathNode*>& path_nodes);
	~PathManager();

	/** Returns the PathNode nearest to @a point. Can return null if there are no PathNodes.
	*/
	const PathNode* nearestPathNode(const Math::Point& point) const;
	/** Returns the PathNode nearest to @a point within the radius @a max_distance.
		Can return null if there are no PathNodes within the radius.
	*/
	const PathNode* nearestPathNode(const Math::Point& point, double max_distance) const;
};

}  // namespace Map
}  // namespace Project

#endif
