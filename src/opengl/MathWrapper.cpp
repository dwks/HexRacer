#include "OpenGL.h"

#include "MathWrapper.h"

namespace Project {
namespace OpenGL {

void MathWrapper::glVertex(const Math::Point &point) {
    glVertex3d(point.getX(), point.getY(), point.getZ());
}

void MathWrapper::glNormal(const Math::Point &point) {
    glNormal3d(point.getX(), point.getY(), point.getZ());
}

void MathWrapper::glMultMatrix(const Math::Matrix &matrix) {
    glMultMatrixd(
        static_cast<const GLdouble *>(matrix.getData()));
}

void MathWrapper::glGetDouble(Math::Matrix &matrix, int openglMatrix) {
    glGetDoublev(openglMatrix,
        static_cast<GLdouble *>(matrix.getData()));
}

void MathWrapper::glTranslate(const Math::Point &point) {
	glTranslatef(point.getX(), point.getY(), point.getZ());
}

}  // namespace OpenGL
}  // namespace Project
