#include "PathManager.h"
#include "math/BoundingBox3D.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Map {

	PathManager::PathManager(const vector<PathNode*>& path_nodes) {

		vector<ObjectSpatial*> objects;
		BoundingBox3D path_bound;
		if (path_nodes.size() > 0) {
			path_bound.setToObject(*path_nodes[0]);
			for (unsigned int i = 0; i < path_nodes.size(); i++) {
				path_bound.expandToInclude(*path_nodes[i]);
				objects.push_back(path_nodes[i]);
			}
		}

		pathTree = new BSPTree3D(path_bound, BSPTree3D::LARGEST_AXIS, 10);
		pathTree->add(objects);

	}

	PathManager::~PathManager() {
		delete(pathTree);
	}

	const PathNode* PathManager::nearestPathNode(const Math::Point& point) const {
		return (PathNode*) pathTree->nearest(point);
	}

	const PathNode* PathManager::nearestPathNode(const Math::Point& point, double max_distance) const {
		return (PathNode*) pathTree->nearest(point, max_distance);
	}





}  // namespace Map
}  // namespace Project
