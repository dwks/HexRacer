#ifndef PROJECT_OPENGL__TRACKBALL_H
#define PROJECT_OPENGL__TRACKBALL_H

#include "math/Matrix.h"
#include "math/Point.h"

namespace Project {
namespace OpenGL {

/** Represents a trackball, keeping track of rotation specified by the user on
    a virtual trackball surface. To use it, pass in the first position the
    mouse clicks at and then all subsequent positions as the mouse moves.
*/
class Trackball {
private:
    Math::Matrix matrix, inverse;
    Math::Point mouseStart, mouseCurrent;
public:
    Trackball();
    
    void reset();
    
    /**
        @param point This should be a 2D coordinate in world space which
            indicates the user's mouse position.
    */
    void setMouseStartAt(Math::Point point);
    void setMouseCurrentAt(Math::Point point);
    
    /** Applies the rotation transformation for this trackball.
    */
    void applyTransform();
    void applyInverseTransform();
    
    /** Multiplies @a vector, which should be of unit length, by the rotation
        transformation matrix. This can be used to obtain a vector pointing in
        the same direction as the viewport, or at right angles, etc.
        
        (Normally multiplying [0, 0, -1, 0] by the matrix will yield a vector
        pointing in the same direction as the viewport. This is because the
        standard OpenGL camera points down the negative Z axis.)
    */
    Math::Point multiplyByMatrix(Math::Point vector);
    Math::Point multiplyByInverseMatrix(Math::Point vector);
private:
    void updateMatrix();
    
    /** Calculates the Z value of the given position (which only has X and
        Y components) on the surface of the trackball. The Z value will be
        in the range [0, 1], and is set to 0 if the position lies outside the
        circumference of the trackball.
    */
    void calculateZOfPoint(Math::Point &position);
};

}  // namespace OpenGL
}  // namespace Project

#endif
