#ifndef TRANSLATEALLDIALOG_H
#define TRANSLATEALLDIALOG_H

#include <QDialog>
#include <QtGui>
#include "math/Point.h"
using namespace Project;
using namespace Math;

class TranslateAllDialog : public QDialog
{
	Q_OBJECT

public:
	TranslateAllDialog(QWidget *parent = 0);
	~TranslateAllDialog();

private:
	QDoubleSpinBox* xBox;
	QDoubleSpinBox* yBox;
	QDoubleSpinBox* zBox;

public:
	Point getTranslation() const { return Math::Point(
		xBox->value(),
		yBox->value(),
		zBox->value()); }
};

#endif // TRANSLATEALLDIALOG_H
