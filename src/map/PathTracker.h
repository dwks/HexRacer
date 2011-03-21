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
	bool finished;
	int ranking;

public:

	PathTracker(const PathManager& _manager);

	void update(Math::Point point);

	const PathNode* getCurrentNode() const { return currentNode; }

	float getLapProgress() const { return progress; }
	float getRaceProgress() const { return (float) numLaps + progress; }
	int getNumLaps() const { return numLaps; }
	bool readyforNewLap() const { return (progress >= 1.0); }

	bool getFinished() const { return finished; }
	int getRanking() const { return ranking; }

	void setFinished(bool _finished) { finished = _finished; }
	void setRanking(int _ranking) { ranking = _ranking; }

	void startNewLap();
	Math::Point getProgressPosition() const { return progressPosition; }
	
	bool atLeastOneNode() const { return manager.getStartNode() != NULL; }

	bool operator < (const PathTracker& other) const;

};

}  // namespace Map
}  // namespace Project

#endif
