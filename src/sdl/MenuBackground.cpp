#include "MenuBackground.h"

#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"

namespace Project {
namespace SDL {

MenuBackground::MenuBackground() {
    rotate = 0.0;
}

void MenuBackground::render() {
    rotate += 0.02;
    glRotated(rotate, 0.0, 0.0, 1.0);
    
    glColor3f(0.0f, 0.2f, 0.8f);
    
    glBegin(GL_TRIANGLE_FAN);
    
    OpenGL::MathWrapper::glVertex(Math::Point(-1.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(0.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-0.4, 1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(0.4, 1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(+1.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(0.4, -1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-0.4, -1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-1.0, 0.0));
    
    glEnd();
}

}  // namespace SDL
}  // namespace Project
