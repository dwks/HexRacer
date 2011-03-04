#ifndef PROJECT_MISC__PROGRESS_TRACKER_H
#define PROJECT_MISC__PROGRESS_TRACKER_H

#include <string>

namespace Project {
namespace Misc {

/* This class receives and processes updates for the progress of time-intensive operations
 * (eg: loading a map)
*/
class ProgressTracker {
public:
	virtual ~ProgressTracker() {}

	virtual void setTotalSteps(int total_steps) {}
	virtual void setCurrentStage(const std::string& stage) {}
	virtual void setCurrentStep(int current_step) {}
};

}  // namespace Misc
}  // namespace Project

#endif
