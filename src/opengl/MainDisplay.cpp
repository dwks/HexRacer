#include <QTimer>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "MainDisplay.h"
#include "log/Logger.h"

#include "MathWrapper.h"

namespace Project {
namespace OpenGL {

const double MainDisplay::FIELD_OF_VIEW = 60.0;

MainDisplay::MainDisplay() {
    scale = 1.0;
    angle = 0.0;
    mouseIsDown = false;
    trackball = new Trackball();
    
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

MainDisplay::~MainDisplay() {
    delete trackball;
}

void MainDisplay::initializeGL() {
    LOG2(OPENGL, INIT, "Initializing OpenGL . . .");
    
    // Depth buffer stuff
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    // Lighting stuff
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Texture stuff
    glEnable(GL_TEXTURE_2D);
    
    // Allow default materials to be used so that colours show up
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Enable line anti-aliasing, so that when glEnable(GL_LINE_SMOOTH) is
    // called, lines will be anti-aliased.
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_LINE_SMOOTH);  // use antialiased lines by default
    
    // Default to smooth shading
    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    
    // Blending settings, for translucency
    glDepthFunc(GL_LEQUAL);
    
    LOG2(OPENGL, INIT, "OpenGL initialization successful");
    
    setViewMatrix();
}

void MainDisplay::resizeGL(int width, int height) {
    LOG2(OPENGL, INIT, "Resizing OpenGL portal to " << width << "x" << height);
    
    OpenGLPortal::resizeGL(width, height);
    
    setViewMatrix();
}

void MainDisplay::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslated(0.0, 0.0, -1.0);
    
    trackball->applyTransform();
    
    /*glTranslated(
        -translation.getX(),
        -translation.getY(),
        -translation.getZ());*/
    
    //glRotated(angle, 0.0, 1.0, 0.0);
    //glScaled(scale, scale, scale);
    
    // add a light
    {
        static GLfloat pos[] = {0.0, 0.0, 2.0, 1.0};
        
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
    }
    
    repaintManager.scheduleNextRepaint(this);
    glFlush();
}

void MainDisplay::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Shift) {
        //LOG(OPENGL, OPENGL, "Shift pressed");
    }
}

void MainDisplay::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Shift) {
        //LOG(OPENGL, OPENGL, "Shift released");
    }
}

void MainDisplay::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        mouseIsDown = true;
        
        Math::Point position = screenToGL(event->pos());
        trackball->setMouseStartAt(position);
    }
}

void MainDisplay::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        mouseIsDown = false;
    }
}

void MainDisplay::mouseMoveEvent(QMouseEvent *event) {
    if(mouseIsDown) {
        Math::Point position = screenToGL(event->pos());
        trackball->setMouseCurrentAt(position);
        
        //updateGL();  // not necessary, continuous update
    }
}

void MainDisplay::wheelEvent(QWheelEvent *event) {
    // mouse wheel delta is defined in multiples of 120
    double move = event->delta() / 15.0 / 8.0;
    
    if(event->modifiers() & Qt::SHIFT) {
        Math::Point axis = Math::Point(0.0, 0.0, 1.0);
        translation += -move * trackball->multiplyByInverseMatrix(axis);
    }
    else {
        if(move > 0.0) {
            scale *= 1.1;
        }
        else {
            scale /= 1.1;
        }
    }
}

void MainDisplay::setViewMatrix() {
    //return;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    double aspect_ratio
        = getCurrentDimensions().getX() / getCurrentDimensions().getY();
    
    gluPerspective(FIELD_OF_VIEW, aspect_ratio, 0.01, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

}  // namespace OpenGL
}  // namespace Project
