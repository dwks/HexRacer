#ifdef CMAKE_BUILD
    #include "hrmemainwindow.moc"
#endif

#include "hrmemainwindow.h"
#include <stdlib.h>
#include "MapObject.h"
#include "LightObject.h"
#include "misc/DirectoryFunctions.h"
using namespace Misc;
#include <vector>

#ifdef WIN32
    #include <direct.h>
#else
    #include "boost/filesystem.hpp"
#endif

using namespace std;

HRMEMainWindow::HRMEMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	meshLoader = new MeshLoader();

	mapEditor = new MapEditorWidget();
	setCentralWidget(mapEditor);
	connect(mapEditor, SIGNAL(selectedObjectChanged(MapObject*)), this, SLOT(setSelectedObject(MapObject*)));
	connect(mapEditor, SIGNAL(propMeshAdded(string)), this, SLOT(addPropMesh(string)));
	connect(mapEditor, SIGNAL(propMeshesChanged(vector<string>)), this, SLOT(setPropMeshes(vector<string>)));

	//
	settingsManager = new SettingsManager("mapeditorconfig.txt");

	/*Menu Bar*************************************************************/

	menuBar = new QMenuBar(this);

	//File Menu
	fileMenu = new QMenu("&File", this);
	fileMenu->addAction("&New", mapEditor, SLOT(newMap()), QKeySequence("CTRL+N"));
	fileMenu->addAction("&Open", this, SLOT(openMapFile()), QKeySequence("CTRL+O"));
	fileMenu->addAction("&Save", mapEditor, SLOT(newFile()), QKeySequence("CTRL+S"));
	fileMenu->addAction("&Save As", this, SLOT(saveMapFileAs()), QKeySequence("CTRL+ALT+S"));

	//Edit Menu
	editMenu = new QMenu("&Edit", this);
	editMenu->addAction("&Delete", mapEditor, SLOT(deleteSelected()), QKeySequence(Qt::Key_Delete));

	//Meshes Menu
	meshMenu = new QMenu("&Mesh", this);

	meshLoadMapper = new QSignalMapper(this);
	connect(meshLoadMapper, SIGNAL(mapped(int)), this, SLOT(loadMesh(int)));
	meshClearMapper = new QSignalMapper(this);
	connect(meshClearMapper, SIGNAL(mapped(int)), this, SLOT(clearMesh(int)));

	for (int i = 0; i < HRMap::NUM_MESHES; i++) {
		HRMap::MeshType type = static_cast<HRMap::MeshType>(i);
		QAction* load_mesh_action = new QAction(QString(("Load "+HRMap::meshTitle(type)).c_str()), this);
		QAction* clear_mesh_action = new QAction(QString(("Clear "+HRMap::meshTitle(type)).c_str()), this);
		meshLoadMapper->setMapping(load_mesh_action, i);
		meshClearMapper->setMapping(clear_mesh_action, i);
		connect(load_mesh_action, SIGNAL(triggered()), meshLoadMapper, SLOT(map()));
		connect(clear_mesh_action, SIGNAL(triggered()), meshClearMapper, SLOT(map()));
		meshMenu->addAction(load_mesh_action);
		meshMenu->addAction(clear_mesh_action);
	}

	//Map Menu
	mapMenu = new QMenu("&Map", this);
	mapMenu->addAction("&Generate Paint Cells",	mapEditor, SLOT(generatePaint()));
	mapMenu->addAction("&Load Cube Map", mapEditor, SLOT(loadCubeMap()));
	mapMenu->addAction("&Load Prop Mesh", this, SLOT(loadPropMesh()));

	menuBar->addMenu(fileMenu);
	menuBar->addMenu(editMenu);
	menuBar->addMenu(meshMenu);
	menuBar->addMenu(mapMenu);
	setMenuBar(menuBar);

	/*Actions*************************************************************/

	advancedRenderingAction = new QAction("&Advanced Rendering", this);
	advancedRenderingAction->setCheckable(true);
	connect(advancedRenderingAction, SIGNAL(toggled(bool)), mapEditor, SLOT(setAdvancedRendering(bool)));

	orthoCameraAction = new QAction("&Orthographic View", this);
	orthoCameraAction->setCheckable(true);
	orthoCameraAction->setShortcut(QKeySequence(Qt::Key_O));
	connect(orthoCameraAction, SIGNAL(toggled(bool)), mapEditor, SLOT(setOrthoView(bool)));

	showPaintAction = new QAction("&Show Paint", this);
	showPaintAction->setCheckable(true);
	connect(showPaintAction, SIGNAL(toggled(bool)), mapEditor, SLOT(setShowPaint(bool)));

	showInvisibleAction = new QAction("&Show Invisible", this);
	showInvisibleAction->setCheckable(true);
	connect(showInvisibleAction, SIGNAL(toggled(bool)), mapEditor, SLOT(setShowInvisible(bool)));

	//Map Objects
	mapObjectGroup = new QActionGroup(this);
	mapObjectGroup->setExclusive(true);
	connect(mapObjectGroup, SIGNAL(triggered(QAction*)), this, SLOT(selectMapObject(QAction*)));

	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		MapObject::ObjectType type = static_cast<MapObject::ObjectType>(i);
		mapObjectAction[i] = new QAction(QString(MapObject::typeTitle(type).c_str()), this);
		mapObjectAction[i]->setCheckable(true);
		mapObjectGroup->addAction(mapObjectAction[i]);
		mapObjectAction[i]->setShortcut(QKeySequence(Qt::Key_1+i));
		/*
		switch (type) {
			case MapObject::LIGHT:
				mapObjectAction[i]->setShortcut(QKeySequence(Qt::Key_Z));
				break;
			case MapObject::MESH_INSTANCE:
				mapObjectAction[i]->setShortcut(QKeySequence(Qt::Key_X));
				break;
			case MapObject::PATH_NODE:
				mapObjectAction[i]->setShortcut(QKeySequence(Qt::Key_C));
				break;
			case MapObject::START_POINT:
				mapObjectAction[i]->setShortcut(QKeySequence(Qt::Key_V));
				break;
			case MapObject::FINISH_PLANE:
				mapObjectAction[i]->setShortcut(QKeySequence(Qt::Key_B));
				break;
			default:
				break;
		}
		*/
	}
	
	//Edit Modes
	editModeGroup = new QActionGroup(this);
	editModeGroup->setExclusive(true);
	connect(editModeGroup, SIGNAL(triggered(QAction*)), this, SLOT(selectEditMode(QAction*)));

	for (int i = 0; i < MapEditorWidget::NUM_EDIT_MODES; i++) {
		MapEditorWidget::EditMode mode = static_cast<MapEditorWidget::EditMode>(i);
		editModeAction[i] = new QAction(QString(MapEditorWidget::editModeTitle(mode).c_str()), this);
		editModeAction[i]->setCheckable(true);
		editModeAction[i]->setShortcut(QKeySequence(Qt::Key_1+i));
		switch (mode) {
			case MapEditorWidget::EDIT_TRANSLATE:
				editModeAction[i]->setShortcut(QKeySequence(Qt::Key_T));
				break;
			case MapEditorWidget::EDIT_ROTATE:
				editModeAction[i]->setShortcut(QKeySequence(Qt::Key_R));
				break;
			case MapEditorWidget::EDIT_SCALE:
				editModeAction[i]->setShortcut(QKeySequence(Qt::Key_F));
				break;
			case MapEditorWidget::EDIT_LINK:
				editModeAction[i]->setShortcut(QKeySequence(Qt::Key_G));
				break;
			default:
				break;
		}
		if (i == 0)
			editModeAction[i]->setChecked(true);
		editModeGroup->addAction(editModeAction[i]);
	}

	/*Options Toolbar*************************************************************/

	optionsFrame = new QFrame(this);
	optionsBar = new QToolBar(this);

	QHBoxLayout* viewing_layout = new QHBoxLayout(this);
	QHBoxLayout* options_layout = new QHBoxLayout(this);
	QHBoxLayout* objects_layout = new QHBoxLayout(this);
	QHBoxLayout* edit_mode_layout = new QHBoxLayout(this);
	QHBoxLayout* prop_mesh_layout = new QHBoxLayout(this);
	
	//Viewing buttons
	QToolButton* advancedRenderingButton = new QToolButton(this);
	advancedRenderingButton->setDefaultAction(advancedRenderingAction);
	QToolButton* orthoCameraButton = new QToolButton(this);
	orthoCameraButton->setDefaultAction(orthoCameraAction);
	QToolButton* showPaintButton = new QToolButton(this);
	showPaintButton->setDefaultAction(showPaintAction);
	QToolButton* showInvisibleButton = new QToolButton(this);
	showInvisibleButton->setDefaultAction(showInvisibleAction);

	viewing_layout->addWidget(advancedRenderingButton);
	viewing_layout->addWidget(orthoCameraButton);
	viewing_layout->addWidget(showPaintButton);
	viewing_layout->addWidget(showInvisibleButton);

	viewing_layout->setAlignment(Qt::AlignLeft);

	//Object buttons
	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		QToolButton* object_button = new QToolButton(this);
		object_button->setDefaultAction(mapObjectAction[i]);
		objects_layout->addWidget(object_button);
	}

	//Edit Mode Buttons
	for (int i = 0; i < MapEditorWidget::NUM_EDIT_MODES; i++) {
		QToolButton* edit_mode_button = new QToolButton(this);
		edit_mode_button->setDefaultAction(editModeAction[i]);
		edit_mode_layout->addWidget(edit_mode_button);
	}

	//Prop Mesh Box

	propMeshBox = new QComboBox(this);
	connect(propMeshBox, SIGNAL(currentIndexChanged(int)), mapEditor, SLOT(setPropMeshIndex(int)));
	propMeshBox->setMinimumContentsLength(30);

	prop_mesh_layout->addWidget(new QLabel("Prop Mesh:", this));
	prop_mesh_layout->addWidget(propMeshBox);
	
	//Toolbar Setup

	objects_layout->setAlignment(Qt::AlignRight);

	options_layout->addLayout(viewing_layout);
	options_layout->addSpacing(10);
	options_layout->addLayout(objects_layout);
	options_layout->addSpacing(10);
	options_layout->addLayout(edit_mode_layout);
	options_layout->addSpacing(10);
	options_layout->addLayout(prop_mesh_layout);

	optionsFrame->setLayout(options_layout);
	optionsBar->addWidget(optionsFrame);

	addToolBar(Qt::TopToolBarArea, optionsBar);

	/*Properties*************************************************************/

	objectPropertiesBar = new QToolBar(this);

	//Position
	positionPropertyFrame = new QFrame(this);

	double range = 9999999.9;
	double single_step = 0.1;
	int decimals = 5;
	positionXBox = new QDoubleSpinBox(positionPropertyFrame);
	positionXBox->setRange(-range, range);
	positionXBox->setDecimals(decimals);
	positionXBox->setSingleStep(single_step);
	positionYBox = new QDoubleSpinBox(positionPropertyFrame);
	positionYBox->setRange(-range, range);
	positionYBox->setDecimals(decimals);
	positionYBox->setSingleStep(single_step);
	positionZBox = new QDoubleSpinBox(positionPropertyFrame);
	positionZBox->setRange(-range, range);
	positionZBox->setDecimals(decimals);
	positionZBox->setSingleStep(single_step);

	connect(positionXBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setSelectedPositionX(double)));
	connect(positionYBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setSelectedPositionY(double)));
	connect(positionZBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setSelectedPositionZ(double)));

	connect(mapEditor, SIGNAL(selectedPositionXChanged(double)), positionXBox, SLOT(setValue(double)));
	connect(mapEditor, SIGNAL(selectedPositionYChanged(double)), positionYBox, SLOT(setValue(double)));
	connect(mapEditor, SIGNAL(selectedPositionZChanged(double)), positionZBox, SLOT(setValue(double)));

	QGridLayout* position_layout = new QGridLayout(positionPropertyFrame);
	position_layout->addWidget(new QLabel("Position", positionPropertyFrame), 0, 0);
	position_layout->addWidget(new QLabel("X", positionPropertyFrame), 1, 0);
	position_layout->addWidget(positionXBox, 1, 1);
	position_layout->addWidget(new QLabel("Y", positionPropertyFrame), 2, 0);
	position_layout->addWidget(positionYBox, 2, 1);
	position_layout->addWidget(new QLabel("Z", positionPropertyFrame), 3, 0);
	position_layout->addWidget(positionZBox, 3, 1);

	positionPropertyFrame->setLayout(position_layout);

	//Rotation
	rotationPropertyFrame = new QFrame(this);

	range = 360;
	single_step = 0.5;
	decimals = 5;
	rotationYawBox = new QDoubleSpinBox(rotationPropertyFrame);
	rotationYawBox->setRange(0.0, range);
	rotationYawBox->setDecimals(decimals);
	rotationYawBox->setSingleStep(single_step);
	rotationPitchBox = new QDoubleSpinBox(rotationPropertyFrame);
	rotationPitchBox->setRange(0.0, range);
	rotationPitchBox->setDecimals(decimals);
	rotationPitchBox->setSingleStep(single_step);
	rotationRollBox = new QDoubleSpinBox(rotationPropertyFrame);
	rotationRollBox->setRange(0.0, range);
	rotationRollBox->setDecimals(decimals);
	rotationRollBox->setSingleStep(single_step);

	connect(rotationYawBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setRotationYaw(double)));
	connect(rotationPitchBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setRotationPitch(double)));
	connect(rotationRollBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setRotationRoll(double)));

	connect(mapEditor, SIGNAL(selectedRotationYawChanged(double)), rotationYawBox, SLOT(setValue(double)));
	connect(mapEditor, SIGNAL(selectedRotationPitchChanged(double)), rotationPitchBox, SLOT(setValue(double)));
	connect(mapEditor, SIGNAL(selectedRotationRollChanged(double)), rotationRollBox, SLOT(setValue(double)));

	QGridLayout* rotation_layout = new QGridLayout(rotationPropertyFrame);
	rotation_layout->addWidget(new QLabel("Rotation", rotationPropertyFrame), 0, 0);
	rotation_layout->addWidget(new QLabel("Yaw", rotationPropertyFrame), 1, 0);
	rotation_layout->addWidget(rotationYawBox, 1, 1);
	rotation_layout->addWidget(new QLabel("Pitch", rotationPropertyFrame), 2, 0);
	rotation_layout->addWidget(rotationPitchBox, 2, 1);
	rotation_layout->addWidget(new QLabel("Roll", rotationPropertyFrame), 3, 0);
	rotation_layout->addWidget(rotationRollBox, 3, 1);

	rotationPropertyFrame->setLayout(rotation_layout);

	//Scale
	scalePropertyFrame = new QFrame(this);

	range = 1000;
	single_step = 0.05;
	decimals = 5;
	scaleBox = new QDoubleSpinBox(scalePropertyFrame);
	scaleBox->setRange(0.0, range);
	scaleBox->setDecimals(decimals);
	scaleBox->setSingleStep(single_step);

	connect(scaleBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setSelectedScale(double)));
	connect(mapEditor, SIGNAL(selectedScaleChanged(double)), scaleBox, SLOT(setValue(double)));

	QGridLayout* scale_layout = new QGridLayout(scalePropertyFrame);
	scale_layout->addWidget(new QLabel("Scale", scalePropertyFrame), 0, 0);
	scale_layout->addWidget(scaleBox);

	scalePropertyFrame->setLayout(scale_layout);

	//Colors

	colorPropertyFrame = new QFrame(this);
	QGridLayout* color_layout = new QGridLayout(colorPropertyFrame);

	colorPropertyMapper = new QSignalMapper(this);
	connect(colorPropertyMapper, SIGNAL(mapped(int)), this, SLOT(choosePropertyColor(int)));

	color_layout->addWidget(new QLabel("Color", colorPropertyFrame), 0, 0);

	for (int i = 0; i < 3; i++) {

		QString color_name;
		if (i == 0)
			color_name = "Diffuse";
		else if (i == 1)
			color_name = "Specular";
		else
			color_name = "Ambient";

		QPushButton* color_button = new QPushButton(color_name, colorPropertyFrame);
		connect(color_button, SIGNAL(clicked()), colorPropertyMapper, SLOT(map()));
		colorPropertyMapper->setMapping(color_button, i);
		color_layout->addWidget(color_button, 1+i, 0);
	}

	colorPropertyFrame->setLayout(color_layout);

	//Lights
	lightPropertyFrame = new QFrame(this);
	QGridLayout* light_layout = new QGridLayout(lightPropertyFrame);

	lightStrengthBox = new QDoubleSpinBox(lightPropertyFrame);
	connect(lightStrengthBox, SIGNAL(valueChanged(double)), mapEditor, SLOT(setLightStrength(double)));
	lightAttenuationBox = new QCheckBox(lightPropertyFrame);
	connect(lightAttenuationBox, SIGNAL(toggled(bool)), mapEditor, SLOT(setLightHasAttenuation(bool)));

	light_layout->addWidget(new QLabel("Light", lightPropertyFrame), 0, 0);
	light_layout->addWidget(new QLabel("Strength", lightPropertyFrame), 1, 0);
	light_layout->addWidget(lightStrengthBox, 1, 1);
	light_layout->addWidget(new QLabel("Has Attenuation", lightPropertyFrame), 2, 0);
	light_layout->addWidget(lightAttenuationBox, 2, 1);

	lightPropertyFrame->setLayout(light_layout);

	//

	objectPropertiesBar->addWidget(positionPropertyFrame);
	objectPropertiesBar->addWidget(rotationPropertyFrame);
	objectPropertiesBar->addWidget(scalePropertyFrame);
	objectPropertiesBar->addWidget(colorPropertyFrame);
	objectPropertiesBar->addWidget(lightPropertyFrame);

	addToolBar(Qt::LeftToolBarArea, objectPropertiesBar);

	/*File Types and Paths*************************************************************/

#ifdef WIN32
	char currentPath[_MAX_PATH];
	getcwd(currentPath, _MAX_PATH);
    saveDir = QString(currentPath);
    meshDir = QString(currentPath);
	propMeshDir = QString(currentPath);
#else
    std::string currentPath = boost::filesystem::current_path().string();
    saveDir = QString::fromStdString(currentPath);
    meshDir = QString::fromStdString(currentPath);
	propMeshDir = QString::fromStdString(currentPath);
#endif

	mapType = QString("HexRace Map File (*.hrm *.HRM *.hrx *.HRX)");
	meshType = QString("Wavefront OBJ (*.obj *.OBJ)");


	//Initialize

	mapObjectAction[0]->setChecked(true);
	setSelectedObject(NULL);
}

HRMEMainWindow::~HRMEMainWindow()
{

}

void HRMEMainWindow::openMapFile() {

	QString qfilename = QFileDialog::getOpenFileName (0,
		tr("Open Map File"),
		saveDir,
		mapType);

	if (!qfilename.isNull()) {
		saveDir = qfilename;
		mapEditor->loadMap(qfilename.toStdString());
	}

}

void HRMEMainWindow::saveMapFileAs() {

	QString qfilename = QFileDialog::getSaveFileName (0,
		tr("Save Map File"),
		saveDir,
		mapType);

	if (!qfilename.isNull()) {
		saveDir = qfilename;
		mapEditor->saveMapAs(qfilename.toStdString());
	}

}

void HRMEMainWindow::loadMesh(int mesh_index) {
	QString qfilename = QFileDialog::getOpenFileName (0,
		tr("Load Mesh"),
		meshDir,
		meshType);

	if (!qfilename.isNull()) {
		meshDir = qfilename;
		mapEditor->loadMesh(static_cast<HRMap::MeshType>(mesh_index), qfilename.toStdString());
	}
}

void HRMEMainWindow::loadPropMesh() {
	QString qfilename = QFileDialog::getOpenFileName (0,
		tr("Load Prop Mesh"),
		propMeshDir,
		meshType);

	if (!qfilename.isNull()) {
		meshDir = qfilename;
		string filename = qfilename.toStdString();
		string name = DirectoryFunctions::extractFilename(filename, false);
		mapEditor->addPropMesh(name, filename);
	}
}

void HRMEMainWindow::clearMesh(int mesh_index) {
	mapEditor->clearMesh(static_cast<HRMap::MeshType>(mesh_index));
}

void HRMEMainWindow::selectMapObject(QAction* action) {

	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		if (action == mapObjectAction[i]) {
			MapObject::ObjectType type = static_cast<MapObject::ObjectType>(i);
			mapEditor->setMapObjectType(type);

			bool change_edit_mode = !MapEditorWidget::editModeCompatible(type, mapEditor->getEditMode());

			for (int j = 0; j < MapEditorWidget::NUM_EDIT_MODES; j++) {
				MapEditorWidget::EditMode mode = static_cast<MapEditorWidget::EditMode>(j);
				bool enabled = MapEditorWidget::editModeCompatible(type, mode);
				editModeAction[j]->setEnabled(enabled);
				if (change_edit_mode && enabled) {
					editModeAction[j]->setChecked(true);
					mapEditor->setEditMode(mode);
					change_edit_mode = false;
				}
			}
			return;
		}
	}
}

void HRMEMainWindow::selectEditMode(QAction* action) {

	for (int i = 0; i < MapEditorWidget::NUM_EDIT_MODES; i++) {
		if (action == editModeAction[i]) {
			mapEditor->setEditMode(static_cast<MapEditorWidget::EditMode>(i));
			return;
		}
	}
}

void HRMEMainWindow::setSelectedObject(MapObject* selected_object) {

	positionPropertyFrame->setEnabled(false);
	rotationPropertyFrame->setEnabled(false);
	scalePropertyFrame->setEnabled(false);
	colorPropertyFrame->setEnabled(false);
	lightPropertyFrame->setEnabled(false);

	Light* light;

	if (selected_object) {
		switch (selected_object->getType()) {
			case MapObject::LIGHT:
				light = ((LightObject*)selected_object)->getLight();
				lightStrengthBox->setValue(light->getStrength());
				lightAttenuationBox->setChecked(light->getHasAttenuation());
				lightPropertyFrame->setEnabled(true);
				break;

			default:
				break;
		}
		positionPropertyFrame->setEnabled(true);
		colorPropertyFrame->setEnabled(selected_object->hasColors());
		rotationPropertyFrame->setEnabled(selected_object->hasRotation());
		scalePropertyFrame->setEnabled(selected_object->hasScale());
	}
}

void HRMEMainWindow::choosePropertyColor(int color_index) {
	QColor color = QColorDialog::getColor(colorToQColor(mapEditor->getSelectedColor(color_index)));

	if (color.isValid()) {
		mapEditor->setSelectedColor(color_index, QColorToColor(color));
	}
}

void HRMEMainWindow::addPropMesh(string name) {
	propMeshBox->addItem(QString(name.c_str()));
}

void HRMEMainWindow::setPropMeshes(vector<string> prop_mesh_names) {
	propMeshBox->clear();
	for (unsigned int i = 0; i < prop_mesh_names.size(); i++) {
		propMeshBox->addItem(QString(prop_mesh_names[i].c_str()));
	}
}