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
	
	QSignalMapper* meshLoadMapper;
	QSignalMapper* meshClearMapper;

	QMenuBar* menuBar;
	QMenu* fileMenu;
	QMenu* meshMenu;
	QMenu* mapMenu;

	QFrame* optionsFrame;
	QToolBar* optionsBar;
	QAction* advancedRenderingAction;
	QAction* orthoCameraAction;
	QAction* showPaintAction;

	QSignalMapper* mapObjectMapper;

	QActionGroup* mapObjectGroup;
	QAction* mapObjectAction[MapObject::NUM_OBJECT_TYPES];

	SettingsManager* settingsManager;

	QString saveDir;
	QString mapType;
	QString meshDir;
	QString meshType;

private slots:

	void openMapFile();
	void saveMapFileAs();
	void loadMesh(int mesh_index);
	void clearMesh(int mesh_index);
	void selectMapObject(QAction* action);

};

#endif // HRMEMAINWINDOW_H
