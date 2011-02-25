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
	int numLaps;

public:

	PathTracker(const PathManager& _manager);

	void update(Math::Point point);

	const PathNode* getCurrentNode() const { return currentNode; }

	float getLapProgress() const { return progress; }
	float getRaceProgress() const { return (float) numLaps + progress; }
	int getNumLaps() const { return numLaps; }
	bool readyforNewLap() const { return (progress >= 1.0); }

	void startNewLap();
	Math::Point getProgressPosition() const { return progressPosition; }
	
	bool atLeastOneNode() const { return manager.getStartNode() != NULL; }

};

}  // namespace Map
}  // namespace Project

#endif
