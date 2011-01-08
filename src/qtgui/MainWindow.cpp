#include <QSplitter>
#include <QVBoxLayout>
#include <QFormLayout>

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>

#include <QApplication>  // to support quit

#include "MainWindow.h"

#include "log/Logger.h"

namespace Project {
namespace QtGUI {

MainWindow::MainWindow() {
    setGeometry(
#ifdef WIN32
        100, 100,
#else
        0, 0,
#endif
        800, 600);
    setWindowTitle("CPSC 585 project");
    
    setup();
    mainDisplay->setFocus();
}

void MainWindow::setup() {
    QSplitter *splitter = new QSplitter();
    mainDisplay = new OpenGL::MainDisplay();
    
    splitter->addWidget(setupSidePanel());
    
    QSplitter *viewSplitter = new QSplitter(Qt::Vertical);
    viewSplitter->addWidget(mainDisplay);
    
    splitter->addWidget(viewSplitter);
    splitter->setStretchFactor(1, 2);
    
    setCentralWidget(splitter);
    
    //mainDisplay->setMinimumSize(1, 1);
    
    setupMenus();
}

void MainWindow::setupMenus() {
    QMenu *fileMenu = new QMenu("&File");
    
    fileMenu->addAction("&Quit",
        QApplication::instance(), SLOT(quit()), Qt::CTRL + Qt::Key_Q);
    
    menuBar()->addMenu(fileMenu);
}

QWidget *MainWindow::setupSidePanel() {
    sidePanel = new QWidget();
    QVBoxLayout *sidePanelLayout = new QVBoxLayout();
    sidePanel->setLayout(sidePanelLayout);
    
    QPushButton *help = new QPushButton("Help");
    sidePanelLayout->addWidget(help);
    connect(help, SIGNAL(clicked()), this, SLOT(showHelp()));
    
    sidePanelLayout->addStretch(1);
    
    return sidePanel;
}

void MainWindow::showHelp() {
    QMessageBox::information(this, "Help",
        "Sorry, no help yet.");
}

}  // namespace QtGUI
}  // namespace Project
