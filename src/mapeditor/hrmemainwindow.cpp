#include "hrmemainwindow.h"
#include <direct.h>
#include <stdlib.h>
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
	meshMenu->addAction("&Load Track",				this, SLOT(loadTrackMesh()));
	meshMenu->addAction("&Load Invisible Track",	this, SLOT(loadInvisibleTrackMesh()));
	meshMenu->addAction("&Load Solid",				this, SLOT(loadSolidMesh()));
	meshMenu->addAction("&Load Invisible Solid",	this, SLOT(loadInvisibleSolidMesh()));
	meshMenu->addAction("&Load Decor",				this, SLOT(loadDecorMesh()));

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

	/*Options Toolbar*************************************************************/

	optionsFrame = new QFrame(this);
	optionsBar = new QToolBar(this);

	QHBoxLayout* viewing_layout = new QHBoxLayout(this);
	QHBoxLayout* options_layout = new QHBoxLayout(this);
	
	QToolButton* advancedRenderingButton = new QToolButton(this);
	advancedRenderingButton->setDefaultAction(advancedRenderingAction);
	QToolButton* orthoCameraButton = new QToolButton(this);
	orthoCameraButton->setDefaultAction(orthoCameraAction);
	QToolButton* showPaintButton = new QToolButton(this);
	showPaintButton->setDefaultAction(showPaintAction);

	viewing_layout->addWidget(advancedRenderingButton);
	viewing_layout->addWidget(orthoCameraButton);
	viewing_layout->addWidget(showPaintButton);

	options_layout->addLayout(viewing_layout);

	optionsFrame->setLayout(options_layout);
	optionsBar->addWidget(optionsFrame);

	addToolBar(Qt::TopToolBarArea, optionsBar);


	/*File Types and Paths*************************************************************/

	char currentPath[_MAX_PATH];
	getcwd(currentPath, _MAX_PATH);
	saveDir = QString(currentPath);
	meshDir = QString(currentPath);

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

string HRMEMainWindow::meshLoadDialog() {
	QString qfilename = QFileDialog::getOpenFileName (0,
		tr("Load Mesh"),
		meshDir,
		meshType);

	if (!qfilename.isNull()) {
		meshDir = qfilename;
		return qfilename.toStdString();
	}
	else {
		return "";
	}
}

void HRMEMainWindow::loadTrackMesh() {
	mapEditor->loadMesh(HRMap::TRACK, meshLoadDialog());
}
void HRMEMainWindow::loadInvisibleTrackMesh() {
	mapEditor->loadMesh(HRMap::INVIS_TRACK, meshLoadDialog());
}
void HRMEMainWindow::loadSolidMesh() {
	mapEditor->loadMesh(HRMap::SOLID, meshLoadDialog());
}
void HRMEMainWindow::loadInvisibleSolidMesh() {
	mapEditor->loadMesh(HRMap::INVIS_SOLID, meshLoadDialog());
}
void HRMEMainWindow::loadDecorMesh() {
	mapEditor->loadMesh(HRMap::DECOR, meshLoadDialog());
}