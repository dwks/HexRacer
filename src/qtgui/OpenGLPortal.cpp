#include <iostream>  // for debugging
#include "OpenGL.h"

#include "OpenGLPortal.h"

#include "log/Logger.h"
#include "misc/StreamAsString.h"

#include "math/Values.h"

namespace Project {
namespace QtGUI {

void OpenGLPortal::resizeGL(int width, int height) {
    // switch to projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // set repaint rect to encompass entire window
    glViewport(0, 0, width, height);
    
    GLfloat aspect_ratio;
    if(width > height) {
        // the window is wider than it is tall
        aspect_ratio = static_cast<GLfloat>(width) / height;
        gluOrtho2D(-aspect_ratio, +aspect_ratio, -1.0f, 1.0f);
    }
    else {
        // the window is taller than it is wide
        aspect_ratio = static_cast<GLfloat>(height) / width;
        gluOrtho2D(-1.0f, 1.0f, -aspect_ratio, +aspect_ratio);
    }
    
    // switch back to normal matrix
    glMatrixMode(GL_MODELVIEW);
    
    currentDimensions = Math::Point(width, height);
}

Math::Point OpenGLPortal::screenToGL(QPoint position) {
    Math::Point offset;
    double orthoDim = Math::minimum(
        currentDimensions.getX(),
        currentDimensions.getY());
    
    // offset is the value that translates by half the screen in the X and Y
    // dimensions. Since due to resizeGL() above the viewport is set
    // differently when width > height as when height > width, this complex
    // code is necessary.
    //
    // offset is used to change the origin (0, 0) from the upper-left corner
    // to the centre of the screen as is expected in OpenGL unit coordinates.
    if(currentDimensions.getX() > currentDimensions.getY()) {
        offset = Math::Point(
            currentDimensions.getX() / currentDimensions.getY(), -1.0);
    }
    else {
        offset = Math::Point(1.0,
            -currentDimensions.getY() / currentDimensions.getX());
    }
    
    Math::Point unitPos(
        position.x() / orthoDim,
        -position.y() / orthoDim);
    
    return 2.0 * unitPos - offset;
}

// This is just the inverse of the function above
QPoint OpenGLPortal::GLtoScreen(Math::Point point) {
    Math::Point offset;
    double orthoDim = Math::minimum(
        currentDimensions.getX(),
        currentDimensions.getY());
    
    if(currentDimensions.getX() > currentDimensions.getY()) {
        offset = Math::Point(
            currentDimensions.getX() / currentDimensions.getY(), -1.0);
    }
    else {
        offset = Math::Point(1.0,
            -currentDimensions.getY() / currentDimensions.getX());
    }
    
    Math::Point unitPos = point / 2.0 + offset;
    
    return QPoint(
        static_cast<int>(unitPos.getX() * orthoDim),
        static_cast<int>(unitPos.getY() * orthoDim));
}

Math::Point OpenGLPortal::getZOfPoint(QPoint position) {
    Math::Point point(position.x(), position.y());
    
    GLfloat pointZ;
    glReadPixels(position.x(),
        static_cast<int>(currentDimensions.getY() - position.y()),
        1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pointZ);
    
    point.setZ(pointZ);
    
    return point;
}

Math::Point OpenGLPortal::unprojectPointWithZ(Math::Point point) {
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    GLdouble model[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    
    GLint view[4];
    glGetIntegerv(GL_VIEWPORT, view);
    
    point.setY(view[3] - point.getY());
    
    double x, y, z;
    gluUnProject(point.getX(), point.getY(), point.getZ(),
        model, projection, view, &x, &y, &z);
    
    return Math::Point(x, y, z);
}

Math::Point OpenGLPortal::unprojectPoint(QPoint position) {
    return unprojectPointWithZ(getZOfPoint(position));
}

QPoint OpenGLPortal::projectPoint(Math::Point point) {
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    GLdouble model[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    
    GLint view[4];
    glGetIntegerv(GL_VIEWPORT, view);
    
    double x, y, z;  // z unused
    gluProject(point.getX(), point.getY(), point.getZ(),
        model, projection, view, &x, &y, &z);
    
    return QPoint(
        static_cast<int>(x),
        static_cast<int>(view[3] - y));
}

}  // namespace QtGUI
}  // namespace Project
