#ifndef PROJECT_MISC__PROGRESS_LOGGER_H
#define PROJECT_MISC__PROGRESS_LOGGER_H

#include "misc/ProgressTracker.h"

namespace Project {
namespace Misc {

class ProgressLogger
	: public ProgressTracker {
private:
	double progressLogInterval;
	int nextLogStep;
	int currentStep;
	int totalSteps;

	void logProgress();
public:

	ProgressLogger(double progress_log_interval = 0.1);

	void setTotalSteps(int total_steps);
	void setCurrentStage(const std::string& stage);
	void setCurrentStep(int current_step);
	void incrementStep();
};

}  // namespace Misc
}  // namespace Project

#endif
