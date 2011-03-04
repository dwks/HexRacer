#ifndef PROJECT_WIDGET__PROGRESS_BAR_WIDGET_H
#define PROJECT_WIDGET__PROGRESS_BAR_WIDGET_H

#include "AbstractWidget.h"
#include "misc/ProgressTracker.h"

namespace Project {
namespace Widget {

	class ProgressBarWidget
		: public AbstractWidget, public Misc::ProgressTracker {
private:
	int totalSteps;
	int currentStep;
public:
	ProgressBarWidget(const std::string &name, const WidgetRect &rect);

	void setTotalSteps(int total_steps) { totalSteps = total_steps; }
	//void setCurrentStage(const std::string& stage);
	void setCurrentStep(int current_step);

	void render();

	virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
