#include "ProgressLogger.h"
#include "log/Logger.h"
#include "math/Values.h"

namespace Project {
namespace Misc {

	ProgressLogger::ProgressLogger(double progress_log_interval) {
		progressLogInterval = progress_log_interval;
		nextLogStep = 0;
		currentStep = 0;
		totalSteps = 1;
	}

	void ProgressLogger::setTotalSteps(int total_steps) {
		totalSteps = total_steps;
		nextLogStep = 0;
		currentStep = 0;
	}

	void ProgressLogger::setCurrentStage(const std::string& stage) {
		LOG(GLOBAL, stage);
	}

	void ProgressLogger::setCurrentStep(int current_step) {
		currentStep = current_step;
		logProgress();
	}

	void ProgressLogger::incrementStep() {
		currentStep++;
		logProgress();
	}

	void ProgressLogger::logProgress() {
		if (currentStep >= nextLogStep) {

			double progress = (double) currentStep / (double) totalSteps;

			LOG(GLOBAL, "Progress: step " << currentStep << '/' << totalSteps
				<< " - " << Math::roundToInt(progress * 100.0) << '%');

			nextLogStep = Math::minimum(totalSteps,
				(int) (std::ceil((progress+progressLogInterval)*totalSteps))
				);
		}
	}

}  // namespace Misc
}  // namespace Project
