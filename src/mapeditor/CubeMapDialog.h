#ifndef CUBEMAPDIALOG_H
#define CUBEMAPDIALOG_H

#include <QtGui>
#include "render/CubeMapFile.h"
using namespace Project;
using namespace Render;


class CubeMapDialog : public QDialog
{
	Q_OBJECT

public:
	CubeMapDialog(const CubeMapFile& file = CubeMapFile(), QWidget *parent = 0);
	~CubeMapDialog();

	static CubeMapFile showCubeMapDialog(const CubeMapFile& file = CubeMapFile(), QWidget *parent = 0);
	CubeMapFile getCubeMapFile() const;

	//QSize sizeHint() const;

private:

	QLineEdit* directoryBox [6];
	
};

#endif // CUBEMAPDIALOG_H
