#ifndef HRMEMAINWINDOW_H
#define HRMEMAINWINDOW_H

#include <QtGui>
#include <QtGui/QMainWindow>
#include "MapEditorWidget.h"
#include "mesh/MeshLoader.h"
#include "opengl/Color.h"
#include "settings/SettingsManager.h"
using namespace Project;
using namespace Render;
using namespace OpenGL;
using namespace Settings;
using namespace Mesh;
using namespace std;

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

	QAction* saveAction;

	QMenuBar* menuBar;
	QMenu* fileMenu;
	QMenu* editMenu;
	QMenu* meshMenu;
	QMenu* mapMenu;

	QFrame* optionsFrame;
	QToolBar* optionsBar;

	QComboBox* propMeshBox;

	//Properties

	QFrame* positionPropertyFrame;
	QFrame* rotationPropertyFrame;
	QFrame* scalePropertyFrame;
	QFrame* colorPropertyFrame;
	QSignalMapper* colorPropertyMapper;

	QFrame* lightPropertyFrame;
	QFrame* meshInstancePropertyFrame;
	
	QToolBar* objectPropertiesBar;

	//Position

	QDoubleSpinBox* positionXBox;
	QDoubleSpinBox* positionYBox;
	QDoubleSpinBox* positionZBox;

	//Rotation

	QDoubleSpinBox* rotationYawBox;
	QDoubleSpinBox* rotationPitchBox;
	QDoubleSpinBox* rotationRollBox;

	//Scale

	QDoubleSpinBox* scaleBox;

	//Light

	QDoubleSpinBox* lightStrengthBox;
	QCheckBox* lightAttenuationBox;

	//Mesh Instance

	QComboBox* instanceTypeBox;

	//Viewing

	QAction* advancedRenderingAction;
	QAction* orthoCameraAction;
	QAction* showPaintAction;
	QAction* showInvisibleAction;

	QActionGroup* mapObjectGroup;
	QAction* mapObjectAction[MapObject::NUM_OBJECT_TYPES];

	QActionGroup* editModeGroup;
	QAction* editModeAction[MapEditorWidget::NUM_EDIT_MODES];

	SettingsManager* settingsManager;

	QString saveDir;
	QString mapType;
	QString meshDir;
	QString propMeshDir;
	QString meshType;
	QString pngType;

private slots:

	void newMap();
	void openMapFile();
	void saveMapFileAs();
	void loadMesh(int mesh_index);
	void clearMesh(int mesh_index);
	void loadPropMesh();
	void selectMapObject(QAction* action);
	void selectEditMode(QAction* action);
	void save2DMapImage();

	void setSelectedObject(MapObject* selected_object);
	void choosePropertyColor(int color_index);
	void addPropMesh(string name);
	void setPropMeshes(vector<string> prop_mesh_names);

};

#endif // HRMEMAINWINDOW_H
