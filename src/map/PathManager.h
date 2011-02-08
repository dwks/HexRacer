#ifndef PROJECT_MAP__PATH_MANAGER_H
#define PROJECT_MAP__PATH_MANAGER_H

#include "PathNode.h"
#include "math/BSPTree3D.h"
#include "config.h"

namespace Project {
namespace Map {

//Relies on HRMap to be loaded!
class PathManager {
private:
	Math::BSPTree3D* pathTree;
	PathNode* startNode;
public:

	struct PathBranch {
		PathNode* start;
		PathNode* end;
		int num_branches;
	};

	PathManager(const vector<PathNode*>& path_nodes);
	~PathManager();

	/** Returns the PathNode nearest to @a point. Can return null if there are no PathNodes.
	*/
	const PathNode* nearestPathNode(const Math::Point& point) const;
	/** Returns the PathNode nearest to @a point within the radius @a max_distance.
		Can return null if there are no PathNodes within the radius.
	*/
	const PathNode* nearestPathNode(const Math::Point& point, double max_distance) const;

	static bool calculatePathProgress(PathNode* start, PathNode* end, int starting_branch = 0);

	const PathNode* getStartNode() const { return startNode; }
};

}  // namespace Map
}  // namespace Project

#endif
