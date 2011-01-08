#include <QTimer>
#include "RepaintManager.h"

namespace Project {
namespace QtGUI {

void RepaintManager::scheduleNextRepaint(QGLWidget *object) {
    if(repaintTimer.elapsed() >= MSEC_BETWEEN_FRAMES) {
        QTimer::singleShot(MSEC_BETWEEN_FRAMES, object, SLOT(updateGL()));
        
        repaintTimer.restart();
    }
}

}  // namespace QtGUI
}  // namespace Project
