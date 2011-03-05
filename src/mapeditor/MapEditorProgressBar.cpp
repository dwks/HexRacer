#include "MapEditorProgressBar.h"

MapEditorProgressBar::MapEditorProgressBar()
{
	dialog = NULL;
}

MapEditorProgressBar::~MapEditorProgressBar(void)
{
}

void MapEditorProgressBar::setTotalSteps(int total_steps) { 
	if (dialog)
		dialog->setMaximum(total_steps);
}
void MapEditorProgressBar::setCurrentStage(const std::string& stage) {
	if (dialog)
		dialog->setLabelText(QString(stage.c_str()));
}
void MapEditorProgressBar::setCurrentStep(int current_step) {
	if (dialog)
		dialog->setValue(current_step);
}

void MapEditorProgressBar::incrementStep() {
	if (dialog)
		dialog->setValue(dialog->value()+1);
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