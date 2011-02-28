#ifndef SCALEALLDIALOG_H
#define SCALEALLDIALOG_H

#include <QtGui>
#include "math/Point.h"
using namespace Project;
using namespace Math;

class ScaleAllDialog : public QDialog
{
	Q_OBJECT

private:
	QDoubleSpinBox* scaleBox;
	QDoubleSpinBox* originXBox;
	QDoubleSpinBox* originYBox;
	QDoubleSpinBox* originZBox;

public:
	ScaleAllDialog(QWidget *parent = 0);
	~ScaleAllDialog();

	double getScale() const { return scaleBox->value(); }
	Point getOrigin() const { return Math::Point(
		originXBox->value(),
		originYBox->value(),
		originZBox->value()); }


	
};

#endif // SCALEALLDIALOG_H
