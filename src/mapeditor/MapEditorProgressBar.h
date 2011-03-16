#pragma once

#include "misc/ProgressTracker.h"
#include <QtGui>

class MapEditorProgressBar : public Project::Misc::ProgressTracker
{
private:
	QProgressDialog* dialog;

	int currentStep;
	int totalSteps;
	int nextUpdateStep;
	void update();

public:
	MapEditorProgressBar(void);
	~MapEditorProgressBar(void);

	void setTotalSteps(int total_steps);
	void setCurrentStage(const std::string& stage);
	void setCurrentStep(int current_step);
	void incrementStep();

	void open();
	void close();
};
