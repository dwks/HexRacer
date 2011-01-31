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

	QString directory;

	//QSize sizeHint() const;

private:

	QLineEdit* directoryBox [6];

private slots:
	void setFilename(int side_index);
};

#endif // CUBEMAPDIALOG_H
