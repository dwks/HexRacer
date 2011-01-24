#ifndef HRMEMAINWINDOW_H
#define HRMEMAINWINDOW_H

#include <QtGui/QMainWindow>
#include "MapEditorWidget.h"
#include "render/MeshLoader.h"
#include "settings/SettingsManager.h"
using namespace Project;
using namespace Render;
using namespace Settings;

class HRMEMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	HRMEMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~HRMEMainWindow();

	QSize sizeHint() const { return QSize(1024, 768); }
	QSize minimumSize () const { return QSize(640, 480); }

private:

	MapEditorWidget* mapEditor;
	MeshLoader* meshLoader;

	QMenuBar* menuBar;
	QMenu* fileMenu;
	QMenu* meshMenu;

	QFrame* optionsFrame;
	QToolBar* optionsBar;
	QAction* advancedRenderingAction;
	QAction* orthoCameraAction;

	SettingsManager* settingsManager;

	QString saveDir;
	QString mapType;
	QString meshDir;
	QString meshType;

private slots:

	void openMapFile();
	void saveMapFileAs();
	string meshLoadDialog();
	void loadTrackMesh();
	void loadInvisibleTrackMesh();
	void loadSolidMesh();
	void loadInvisibleSolidMesh();
	void loadDecorMesh();

};

#endif // HRMEMAINWINDOW_H
