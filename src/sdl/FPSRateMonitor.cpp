#include "FPSRateMonitor.h"

#include "widget/NormalTextLayout.h"

#include "misc/StreamAsString.h"
#include "misc/Sleeper.h"

namespace Project {
namespace SDL {

FPSRateMonitor::FPSRateMonitor() {
    startTime = 0;
    frames = 0;
    
    widget = new Widget::TextWidget("fps", "",
        Widget::NormalTextLayout::ALIGN_RIGHT,
        Widget::WidgetRect(0.0, 0.95, 0.125, 0.04));
}

FPSRateMonitor::~FPSRateMonitor() {
    delete widget;
}

void FPSRateMonitor::countFrame() {
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    if(!startTime) {
        startTime = now;
    }
    else if(startTime + FPS_UPDATE_DISPLAY <= now) {
        startTime += FPS_UPDATE_DISPLAY;
        
        Misc::StreamAsString stream;
        stream << frames << " fps";
        widget->setText(stream);
        
        if(frames < 20) {
            widget->setColor(OpenGL::Color::RED);
        }
        else if(frames < 60) {
            widget->setColor(OpenGL::Color::WHITE);
        }
        else {
            widget->setColor(OpenGL::Color::GREEN);
        }
        
        frames = 0;
    }
    
    frames ++;
}

}  // namespace SDL
}  // namespace Project
