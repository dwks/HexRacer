#include "PathTracker.h"
#include "math/Geometry.h"

#include "log/Logger.h"

namespace Project {
namespace Map {

	PathTracker::PathTracker(const PathManager& _manager)
		: manager(_manager) {
		progress = 0.0;
		currentNode = manager.getStartNode();
	}

	void PathTracker::update(Math::Point point) {

		currentNode = manager.nearestPathNode(point);

		float node_progress = getWrappedProgress(currentNode->getProgress());

		std::vector<PathPair> pairs;

		for (unsigned int i = 0; i < currentNode->getNextNodes().size(); i++) {
			//Add all current-next pairs as candidates
			PathPair pair;
			pair.start = currentNode;
			pair.end = currentNode->getNextNodes()[i];
			pairs.push_back(pair);
		}
		for (unsigned int i = 0; i < currentNode->getPreviousNodes().size(); i++) {
			//Add all previous-current pairs as candidates
			PathPair pair;
			pair.start = currentNode->getPreviousNodes()[i];
			pair.end = currentNode;
			pairs.push_back(pair);
		}

		if (pairs.empty()) {
			LOG(WORLD, "ERROR: Current path node has no links!");
			return;
		}

		//Find the best node pair based on distance from the line between them
		PathPair best_pair;
		double min_distance_squared = 0.0;
		for (unsigned int i = 0; i < pairs.size(); i++) {

			Math::Point p;
			Math::Geometry::intersectLine3D(
				pairs[i].start->getPosition(),
				pairs[i].end->getPosition(),
				point,
				&p);
			/*
			Math::Geometry::intersectLineSegment(
				pairs[i].start->getPosition(),
				pairs[i].end->getPosition(),
				point,
				&p);
				*/

			double dist_squared = p.distanceSquared(point);

			if (i == 0 || dist_squared < min_distance_squared) {
				best_pair = pairs[i];
				progressPosition = p;
				min_distance_squared = dist_squared;
			}

		}

		double u = Math::Geometry::getUOfLine(
			best_pair.start->getPosition(),
			best_pair.end->getPosition(),
			progressPosition);

		//Interpolate the progress
		progress = static_cast<float>(
			getWrappedProgress(best_pair.start->getProgress())*(1.0-u) +
			getWrappedProgress(best_pair.end->getProgress())*u);

	}

	float PathTracker::getWrappedProgress(float _prog) {
		if (progress < 0.5f && _prog > 0.5f)
			return -_prog;
		else if (progress > 0.5f && _prog < 0.5f)
			return 1.0f+_prog;
		else
			return _prog;
	}

}  // namespace Map
}  // namespace Project
