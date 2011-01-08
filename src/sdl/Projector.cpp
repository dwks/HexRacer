#include "opengl/OpenGL.h"

#include "Projector.h"
#include "math/Values.h"

namespace Project {
namespace SDL {

Math::Point Projector::screenToGL(Point2D point) {
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
            double(currentDimensions.getX()) / currentDimensions.getY(), -1.0);
    }
    else {
        offset = Math::Point(1.0,
            -double(currentDimensions.getY()) / currentDimensions.getX());
    }
    
    Math::Point unitPos(
        point.getX() / orthoDim,
        -point.getY() / orthoDim);
    
    return 2.0 * unitPos - offset;
}

// This is just the inverse of the function above
Point2D Projector::GLtoScreen(Math::Point point) {
    Math::Point offset;
    double orthoDim = Math::minimum(
        currentDimensions.getX(),
        currentDimensions.getY());
    
    if(currentDimensions.getX() > currentDimensions.getY()) {
        offset = Math::Point(
            double(currentDimensions.getX()) / currentDimensions.getY(), -1.0);
    }
    else {
        offset = Math::Point(1.0,
            -double(currentDimensions.getY()) / currentDimensions.getX());
    }
    
    Math::Point unitPos = point / 2.0 + offset;
    
    return Point2D(
        static_cast<int>(unitPos.getX() * orthoDim),
        static_cast<int>(unitPos.getY() * orthoDim));
}

Math::Point Projector::getZOfPoint(Point2D point) {
    GLfloat pointZ;
    glReadPixels(point.getX(),
        currentDimensions.getY() - point.getY(),
        1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pointZ);
    
    return Math::Point(point.getX(), point.getY(), pointZ);
}

Math::Point Projector::unprojectPointWithZ(Math::Point point) {
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

Math::Point Projector::unprojectPoint(Point2D point) {
    return unprojectPointWithZ(getZOfPoint(point));
}

Point2D Projector::projectPoint(Math::Point point) {
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    GLdouble model[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    
    GLint view[4];
    glGetIntegerv(GL_VIEWPORT, view);
    
    double x, y, z;  // z unused
    gluProject(point.getX(), point.getY(), point.getZ(),
        model, projection, view, &x, &y, &z);
    
    return Point2D(
        static_cast<int>(x),
        static_cast<int>(view[3] - y));
}

}  // namespace SDL
}  // namespace Project
