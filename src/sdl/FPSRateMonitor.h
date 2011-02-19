#ifndef PROJECT_SDL__FPSRATE_MONITOR_H
#define PROJECT_SDL__FPSRATE_MONITOR_H

#include "widget/TextWidget.h"

namespace Project {
namespace SDL {

class FPSRateMonitor {
private:
    static const int FPS_UPDATE_DISPLAY = 1000;
private:
    unsigned long startTime;
    int frames;
    Widget::TextWidget *widget;
public:
    FPSRateMonitor();
    
    void countFrame();
    
    Widget::TextWidget *getWidget() { return widget; }
};

}  // namespace SDL
}  // namespace Project

#endif
