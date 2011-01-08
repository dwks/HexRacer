#ifndef PROJECT_OPENGL__MAIN_DISPLAY_H
#define PROJECT_OPENGL__MAIN_DISPLAY_H

#include "qtgui/OpenGLPortal.h"
#include "qtgui/RepaintManager.h"
#include "Trackball.h"

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace Project {
namespace OpenGL {

class MainDisplay : public QtGUI::OpenGLPortal {
    Q_OBJECT
private:
    static const double FIELD_OF_VIEW;
private:
    QtGUI::RepaintManager repaintManager;
    Trackball *trackball;
    Math::Point translation;
    double scale, angle;
    bool mouseIsDown;
public:
    MainDisplay();
    ~MainDisplay();
    
    virtual void initializeGL();
    
    virtual void resizeGL(int width, int height);
    
    virtual void paintGL();
public:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    
    virtual void wheelEvent(QWheelEvent *event);
private:
    void setViewMatrix();
};

}  // namespace OpenGL
}  // namespace Project

#endif
