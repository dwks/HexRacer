#ifndef MAPOPTIONSDIALOG_H
#define MAPOPTIONSDIALOG_H

#include <QtGui>
#include "map/MapOptions.h"
#include <string>
using namespace Project;
using namespace Map;

class MapOptionsDialog : public QDialog
{
	Q_OBJECT

public:
	MapOptionsDialog(const MapOptions& options, QWidget *parent = 0);
	~MapOptionsDialog();

	static void showOptionsDialog(MapOptions& options, QWidget *parent = 0);

private:
	QCheckBox* bgBloomBox;
	QSpinBox* numLapsBox;
};

#endif // MAPOPTIONSDIALOG_H
