#ifndef PROJECT_SDL__PROJECTOR_H
#define PROJECT_SDL__PROJECTOR_H

#include "math/Point.h"
#include "Point2D.h"

namespace Project {
namespace SDL {

/** Performs projection and unprojection between screen and OpenGL coordinates.
*/
class Projector {
private:
    Point2D currentDimensions;
public:
    const Point2D &getCurrentDimensions() const
        { return currentDimensions; }
    
    void setCurrentDimensions(const Point2D &dimensions)
        { currentDimensions = dimensions; }
    
    /** Converts the given point on the screen into the (x, y) equivalent
        in world unit coordinates, leaving the Z coordinate unset.
    */
    Math::Point screenToGL(Point2D point);
    
    /** Given a 2D point on the screen, finds the corresponding 3D point of
        the object under the mouse cursor. (This uses the Z buffer to calculate
        the Z-coordinate of the point in world space.)
    */
    Math::Point unprojectPoint(Point2D point);
    
    /** Given a 3D point in world space, finds the 2D point on the screen that
        this point would appear at. Note that this position is not guaranteed
        to actually be visible or within the bounds of the screen.
    */
    Point2D projectPoint(Math::Point point);
protected:
    Point2D GLtoScreen(Math::Point point);
    Math::Point getZOfPoint(Point2D point);
    Math::Point unprojectPointWithZ(Math::Point point);
};

}  // namespace SDL
}  // namespace Project

#endif
