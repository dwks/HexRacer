#include "PathTracker.h"
#include "math/Geometry.h"
#include "math/Values.h"
#include <math.h>
#include "log/Logger.h"

namespace Project {
namespace Map {

	PathTracker::PathTracker(const PathManager& _manager)
		: manager(_manager) {
		progress = 0.0;
		currentNode = manager.getStartNode();
		numLaps = 0;
	}

	void PathTracker::update(Math::Point point) {

		currentNode = manager.nearestPathNode(point);

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
			LOG2(WORLD, ERROR, "Path Node #" << currentNode->index << " has no links!" );
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
 
			double dist_squared = p.distanceSquared(point);

			if (i == 0 || dist_squared < min_distance_squared) {
				best_pair = pairs[i];
				progressPosition = p;
				min_distance_squared = dist_squared;
			}

		}

		if (best_pair.start->getProgress() > best_pair.end->getProgress())
			progress = getWrappedProgress(Math::maximum(best_pair.start->getProgress(), best_pair.end->getProgress()));
		else {

			float u = static_cast<float>(
				Math::Geometry::getUOfLine(
				best_pair.start->getPosition(),
				best_pair.end->getPosition(),
				progressPosition)
				);

			//Interpolate the progress
			progress = static_cast<float>(
				getWrappedProgress(
					best_pair.start->getProgress()*(1.0-u) +
					best_pair.end->getProgress()*u
					)
					);

		}

	}

	void PathTracker::startNewLap() {
		numLaps++;
		progress = 0.0f;
	}

	float PathTracker::getWrappedProgress(float _prog) {
		if (fabs(_prog - progress) >= 0.5f) {
			if (progress < 0.5f && _prog > 0.5f)
				return _prog-1.0f;
			else if (progress > 0.5f && _prog < 0.5f)
				return 1.0f+_prog;
		}
		
		return _prog;
	}

}  // namespace Map
}  // namespace Project
