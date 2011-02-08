#ifndef PROJECT_MAP__PATH_TRACKER_H
#define PROJECT_MAP__PATH_TRACKER_H

#include "PathManager.h"
#include "PathNode.h"
#include "math/Point.h"

namespace Project {
namespace Map {

class PathTracker {
private:

	struct PathPair {
		const PathNode* start;
		const PathNode* end;
	};

	const PathManager& manager;
	float progress;
	const PathNode* currentNode;
	float getWrappedProgress(float _prog);

	Math::Point progressPosition;
public:

	PathTracker(const PathManager& _manager);

	void update(Math::Point point);

	const PathNode* getCurrentNode() const { return currentNode; }
	float getProgress() const { return progress; }
	bool isReadyforNewLap() const { return (progress >= 1.0f); }
	Math::Point getProgressPosition() const { return progressPosition; }

};

}  // namespace Map
}  // namespace Project

#endif
