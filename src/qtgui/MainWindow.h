#ifndef PROJECT_QTGUI__MAIN_WINDOW_H
#define PROJECT_QTGUI__MAIN_WINDOW_H

#include <string>

#include <QMainWindow>
#include "opengl/MainDisplay.h"

class QPushButton;

namespace Project {
namespace QtGUI {

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QWidget *sidePanel;
    OpenGL::MainDisplay *mainDisplay;
public:
    MainWindow();
private slots:
    void showHelp();
private:
    void setup();
    void setupMenus();
    QWidget *setupSidePanel();
};

}  // namespace QtGUI
}  // namespace Project

#endif
