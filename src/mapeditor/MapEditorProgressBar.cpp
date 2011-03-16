#include "MapEditorProgressBar.h"
#include "math/Values.h"

MapEditorProgressBar::MapEditorProgressBar()
{
	dialog = NULL;
	nextUpdateStep = 0;
	currentStep = 0;
	totalSteps = 1;
}

MapEditorProgressBar::~MapEditorProgressBar(void)
{
}

void MapEditorProgressBar::setTotalSteps(int total_steps) { 
	if (dialog)
		dialog->setMaximum(total_steps);
	totalSteps = total_steps;
	nextUpdateStep = 0;
}
void MapEditorProgressBar::setCurrentStage(const std::string& stage) {
	if (dialog)
		dialog->setLabelText(QString(stage.c_str()));
}
void MapEditorProgressBar::setCurrentStep(int current_step) {
	currentStep = current_step;
	update();
}

void MapEditorProgressBar::incrementStep() {
	currentStep++;
	update();
}

void MapEditorProgressBar::open() {
	dialog = new QProgressDialog(QString(), QString(), 0, 100);
	dialog->open();
	dialog->setValue(0);
}
void MapEditorProgressBar::close() {
	delete dialog;
	dialog = NULL;
}

void MapEditorProgressBar::update() {
	if (currentStep >= nextUpdateStep && dialog) {

		dialog->setValue(currentStep);

		double progress = (double) currentStep / (double) totalSteps;

		nextUpdateStep = Project::Math::minimum(totalSteps,
				(int) (std::ceil((progress+0.01)*totalSteps))
				);
	}
}