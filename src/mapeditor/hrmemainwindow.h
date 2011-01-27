#ifndef HRMEMAINWINDOW_H
#define HRMEMAINWINDOW_H

#include <QtGui>
#include <QtGui/QMainWindow>
#include "MapEditorWidget.h"
#include "render/MeshLoader.h"
#include "opengl/Color.h"
#include "settings/SettingsManager.h"
using namespace Project;
using namespace Render;
using namespace OpenGL;
using namespace Settings;

class HRMEMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	HRMEMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~HRMEMainWindow();

	QSize sizeHint() const { return QSize(1024, 768); }
	QSize minimumSize () const { return QSize(640, 480); }

	static QColor colorToQColor(Color color) {
		return QColor(color.redi(), color.greeni(), color.bluei(), color.alphai());
	}

	static Color QColorToColor(QColor color) {
		return Color::colori(color.red(), color.green(), color.blue(), color.alpha());
	}

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

	//Properties

	QFrame* positionPropertyFrame;
	QFrame* colorPropertyFrame;
	QSignalMapper* colorPropertyMapper;

	QFrame* lightPropertyFrame;
	
	
	QToolBar* objectPropertiesBar;

	QDoubleSpinBox* positionXBox;
	QDoubleSpinBox* positionYBox;
	QDoubleSpinBox* positionZBox;

	//Viewing

	QAction* advancedRenderingAction;
	QAction* orthoCameraAction;
	QAction* showPaintAction;

	QActionGroup* mapObjectGroup;
	QAction* mapObjectAction[MapObject::NUM_OBJECT_TYPES];

	QActionGroup* editModeGroup;
	QAction* editModeAction[MapEditorWidget::NUM_EDIT_MODES];

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
	void selectEditMode(QAction* action);

	void selectedObjectChanged(MapObject* selected_object);

	void choosePropertyColor(int color_index);

};

#endif // HRMEMAINWINDOW_H
