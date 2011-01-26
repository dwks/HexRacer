#ifdef CMAKE_BUILD
    #include "hrmemainwindow.moc"
#endif

#include "hrmemainwindow.h"
#include <stdlib.h>
#include "MapObject.h"

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
	/*
	meshMenu->addAction("&Load Track",				this, SLOT(loadTrackMesh()));
	meshMenu->addAction("&Load Invisible Track",	this, SLOT(loadInvisibleTrackMesh()));
	meshMenu->addAction("&Load Solid",				this, SLOT(loadSolidMesh()));
	meshMenu->addAction("&Load Invisible Solid",	this, SLOT(loadInvisibleSolidMesh()));
	meshMenu->addAction("&Load Decor",				this, SLOT(loadDecorMesh()));
	*/

	//Map Menu
	mapMenu = new QMenu("&Map", this);
	mapMenu->addAction("&Generate Paint Cells",	mapEditor, SLOT(generatePaint()));

	menuBar->addMenu(fileMenu);
	menuBar->addMenu(meshMenu);
	menuBar->addMenu(mapMenu);
	setMenuBar(menuBar);

	/*Actions*************************************************************/

	advancedRenderingAction = new QAction("&Advanced Rendering", this);
	advancedRenderingAction->setCheckable(true);
	connect(advancedRenderingAction, SIGNAL(toggled(bool)), mapEditor, SLOT(setAdvancedRendering(bool)));

	orthoCameraAction = new QAction("&Orthographic View", this);
	orthoCameraAction->setCheckable(true);
	connect(orthoCameraAction, SIGNAL(toggled(bool)), mapEditor, SLOT(setOrthoView(bool)));

	showPaintAction = new QAction("&Show Paint", this);
	showPaintAction->setCheckable(true);
	connect(showPaintAction, SIGNAL(toggled(bool)), mapEditor, SLOT(setShowPaint(bool)));

	//Map Objects
	mapObjectGroup = new QActionGroup(this);
	mapObjectGroup->setExclusive(true);
	connect(mapObjectGroup, SIGNAL(triggered(QAction*)), this, SLOT(selectMapObject(QAction*)));

	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		MapObject::ObjectType type = static_cast<MapObject::ObjectType>(i);
		mapObjectAction[i] = new QAction(QString(MapObject::typeTitle(type).c_str()), this);
		mapObjectAction[i]->setCheckable(true);
		if (i == 0)
			mapObjectAction[i]->setChecked(true);
		mapObjectGroup->addAction(mapObjectAction[i]);
	}

	/*Options Toolbar*************************************************************/

	optionsFrame = new QFrame(this);
	optionsBar = new QToolBar(this);

	QHBoxLayout* viewing_layout = new QHBoxLayout(this);
	QHBoxLayout* options_layout = new QHBoxLayout(this);
	QHBoxLayout* objects_layout = new QHBoxLayout(this);
	
	//Viewing buttons
	QToolButton* advancedRenderingButton = new QToolButton(this);
	advancedRenderingButton->setDefaultAction(advancedRenderingAction);
	QToolButton* orthoCameraButton = new QToolButton(this);
	orthoCameraButton->setDefaultAction(orthoCameraAction);
	QToolButton* showPaintButton = new QToolButton(this);
	showPaintButton->setDefaultAction(showPaintAction);

	viewing_layout->addWidget(advancedRenderingButton);
	viewing_layout->addWidget(orthoCameraButton);
	viewing_layout->addWidget(showPaintButton);

	viewing_layout->setAlignment(Qt::AlignLeft);

	//Object buttons
	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		QToolButton* object_button = new QToolButton(this);
		object_button->setDefaultAction(mapObjectAction[i]);
		objects_layout->addWidget(object_button);
	}

	objects_layout->setAlignment(Qt::AlignRight);

	options_layout->addLayout(viewing_layout);
	options_layout->addSpacing(10);
	options_layout->addLayout(objects_layout);

	optionsFrame->setLayout(options_layout);
	optionsBar->addWidget(optionsFrame);

	addToolBar(Qt::TopToolBarArea, optionsBar);


	/*File Types and Paths*************************************************************/

#ifdef WIN32
	char currentPath[_MAX_PATH];
	getcwd(currentPath, _MAX_PATH);
    saveDir = QString(currentPath);
    meshDir = QString(currentPath);
#else
    std::string currentPath = boost::filesystem::current_path().string();
    saveDir = QString::fromStdString(currentPath);
    meshDir = QString::fromStdString(currentPath);
#endif

	mapType = QString("HexRace Map File (*.hrm *.HRM)");
	meshType = QString("Wavefront OBJ (*.obj *.OBJ)");

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
void HRMEMainWindow::clearMesh(int mesh_index) {
	mapEditor->clearMesh(static_cast<HRMap::MeshType>(mesh_index));
}

void HRMEMainWindow::selectMapObject(QAction* action) {

	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		if (action == mapObjectAction[i]) {
			mapEditor->setMapObjectType(static_cast<MapObject::ObjectType>(i));
			return;
		}
	}
}