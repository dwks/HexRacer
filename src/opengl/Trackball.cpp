#include <cmath>  // for std::sqrt()

#include "OpenGL.h"

#include "Trackball.h"
#include "math/Values.h"
#include "MathWrapper.h"

namespace Project {
namespace OpenGL {

Trackball::Trackball() {
    reset();
}

void Trackball::reset() {
    matrix.setToIdentity();
    inverse.setToIdentity();
}

void Trackball::setMouseStartAt(Math::Point point) {
    mouseStart = point;
    calculateZOfPoint(mouseStart);
}

void Trackball::setMouseCurrentAt(Math::Point point) {
    mouseCurrent = point;
    calculateZOfPoint(mouseCurrent);
    
    updateMatrix();
}

void Trackball::updateMatrix() {
    // axis is perpendicular to both mouseDown and mouseCurrent; it's an axis
    // of rotation to move the sphere from the former to the latter
    Math::Point axis = mouseStart.crossProduct(mouseCurrent);
    
    double length = axis.length();
    if(length < -1.0) length = -1.0;
    if(length > 1.0) length = 1.0;
    
    double angle = std::asin(length);
    angle *= 180.0 / PI;
    
    glPushMatrix();
    
    // update the normal matrix
    glLoadIdentity();
    glRotated(angle, axis.getX(), axis.getY(), axis.getZ());
    MathWrapper::glMultMatrix(matrix);
    MathWrapper::glGetDouble(matrix, GL_MODELVIEW_MATRIX);
    
    // update the inverse matrix
    glLoadIdentity();
    MathWrapper::glMultMatrix(inverse);
    glRotated(-angle, axis.getX(), axis.getY(), axis.getZ());
    MathWrapper::glGetDouble(inverse, GL_MODELVIEW_MATRIX);
    
    glPopMatrix();
    
    mouseStart = mouseCurrent;
}

void Trackball::applyTransform() {
    MathWrapper::glMultMatrix(matrix);
}

void Trackball::applyInverseTransform() {
    MathWrapper::glMultMatrix(inverse);
}

Math::Point Trackball::multiplyByMatrix(Math::Point vector) {
    return matrix * vector;
}

Math::Point Trackball::multiplyByInverseMatrix(Math::Point vector) {
    return inverse * vector;
}

void Trackball::calculateZOfPoint(Math::Point &position) {
    // A 3D unit sphere has the equation
    //     x^2 + y^2 + z^2 = 1^2
    // The below solves the equation for z^2.
    double zSquared = 1.0
        - (position.getX() * position.getX())
        - (position.getY() * position.getY());
    
    if(zSquared < 0.0) {
        // In this case there is no real solution, which means that (x, y) does
        // not lie on the sphere. We default to just setting Z to 0.
        position.setZ(0.0);
    }
    else {
        position.setZ(std::sqrt(zSquared));
    }
}

}  // namespace OpenGL
}  // namespace Project
