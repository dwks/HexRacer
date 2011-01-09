#ifndef PROJECT_OPENGL__MATH_WRAPPER_H
#define PROJECT_OPENGL__MATH_WRAPPER_H

#include "math/Point.h"
#include "math/Point2D.h"
#include "math/Matrix.h"

namespace Project {
namespace OpenGL {

class MathWrapper {
public:
    /** Calls some glVertex*() variant with the data in @a point.
    */
    static void glVertex(const Math::Point &point);

    /** Calls some glNormal*() variant with the data in this vector.
        
        Note: the vector should be normalized before calling this function.
    */
    static void glNormal(const Math::Point &point);
    
    static void glMultMatrix(const Math::Matrix &matrix);
    static void glGetDouble(Math::Matrix &matrix, int openglMatrix);
};

}  // namespace OpenGL
}  // namespace Project

#endif
