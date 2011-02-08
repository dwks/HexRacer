#include "PathManager.h"
#include "math/BoundingBox3D.h"

#include "log/Logger.h"

using namespace Project;
using namespace Math;

namespace Project {
namespace Map {

	PathManager::PathManager(const vector<PathNode*>& path_nodes) {

		vector<ObjectSpatial*> objects;
		BoundingBox3D path_bound;

		startNode = NULL;

		if (path_nodes.size() > 0) {
			for (unsigned int i = 0; i < path_nodes.size(); i++) {
				if (i == 0)
					path_bound.setToObject(*path_nodes[i]);
				else
					path_bound.expandToInclude(*path_nodes[i]);
				
				if (path_nodes[i]->getProgress() <= 0.0f)
					startNode = path_nodes[i];

				objects.push_back(path_nodes[i]);
			}
		}
		else
			LOG2(WORLD, ERROR, "No path nodes defined in map!");

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

	bool PathManager::calculatePathProgress(PathNode* start, PathNode* end, int starting_branch) {

		vector<PathBranch> branches;
		vector<PathNode*> unbranching_nodes;
		
		double total_length = 0.0;
		double progress_start = start->getProgress();
		double progress_delta = end->getProgress()-progress_start;

		int num_branches = 0;
		PathNode* current_node = start;
		PathBranch current_branch;

		while (current_node != end) {

			const vector<PathNode*>& next_nodes = current_node->getNextNodes();
			if (next_nodes.empty())
				return false;

			if (num_branches <= 0) {

				unbranching_nodes.push_back(current_node);

				if (current_node != start) {

					if (next_nodes.size() > 1) {
						//New branch(es)
						num_branches += (static_cast<int>(next_nodes.size())-1);
						current_branch.start = current_node;
						current_branch.num_branches = static_cast<int>(next_nodes.size());
					}

				}

			}
			else {

				const vector<PathNode*>& previous_nodes = current_node->getPreviousNodes();
				num_branches -= (static_cast<int>(previous_nodes.size())-1);

				if (num_branches == 0 && current_node != end) {

					current_branch.end = current_node;
					branches.push_back(current_branch);

					unbranching_nodes.push_back(current_node);

				}

			}

			current_node->setProgress(total_length);

			if (starting_branch >= static_cast<int>(next_nodes.size()))
				starting_branch = 0;

			total_length += current_node->getPosition().distance(next_nodes[starting_branch]->getPosition());
			current_node = next_nodes[starting_branch];

			starting_branch = 0;
		}

		//Set the progress of all unbranching nodes based on their distance
		for (unsigned int i = 0; i < unbranching_nodes.size(); i++) {
			unbranching_nodes[i]->setProgress(
				progress_start + (unbranching_nodes[i]->getProgress()/total_length)*progress_delta
				);
		}

		//Repeat the algorithm for all branches in the path
		for (unsigned int i = 0; i < branches.size(); i++) {
			for (int j = 0; j < branches[i].num_branches; j++) {
				if (!calculatePathProgress(branches[i].start, branches[i].end, j)) {
					return false;
				}
			}
		}

		return true;
	}


}  // namespace Map
}  // namespace Project
