#ifndef PROJECT_QTGUI__OPEN_GLPORTAL_H
#define PROJECT_QTGUI__OPEN_GLPORTAL_H

#include <QGLWidget>
#include <QPoint>

#include "math/Point.h"

namespace Project {
namespace QtGUI {

class OpenGLPortal : public QGLWidget {
private:
    Math::Point currentDimensions;
public:
    virtual void initializeGL() = 0;
    
    virtual void resizeGL(int width, int height);
    
    virtual void paintGL() = 0;
protected:
    const Math::Point &getCurrentDimensions() const
        { return currentDimensions; }
    
    /** Converts the given point on the screen into the (x, y) equivalent
        in world unit coordinates, leaving the Z coordinate unset.
    */
    Math::Point screenToGL(QPoint position);
    
    /** Given a 2D point on the screen, finds the corresponding 3D point of
        the object under the mouse cursor. (This uses the Z buffer to calculate
        the Z-coordinate of the point in world space.)
    */
    Math::Point unprojectPoint(QPoint position);
    
    /** Given a 3D point in world space, finds the 2D point on the screen that
        this point would appear at. Note that this position is not guaranteed
        to actually be visible or within the bounds of the screen.
    */
    QPoint projectPoint(Math::Point point);
protected:
    QPoint GLtoScreen(Math::Point point);
    Math::Point getZOfPoint(QPoint position);
    Math::Point unprojectPointWithZ(Math::Point point);
};

}  // namespace QtGUI
}  // namespace Project

#endif
