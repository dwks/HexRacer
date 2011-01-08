#ifndef PROJECT_QTGUI__REPAINT_MANAGER_H
#define PROJECT_QTGUI__REPAINT_MANAGER_H

#include <QDateTime>
#include <QGLWidget>

namespace Project {
namespace QtGUI {

class RepaintManager {
private:
    static const int MSEC_BETWEEN_FRAMES = 20;
private:
    QTime repaintTimer;
public:
    void scheduleNextRepaint(QGLWidget *object);
};

}  // namespace QtGUI
}  // namespace Project

#endif
