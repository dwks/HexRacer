#ifndef PROJECT_WIDGET__GUISYSTEM_H
#define PROJECT_WIDGET__GUISYSTEM_H

#include "CompositeWidget.h"
#include "render/FontManager.h"

namespace Project {
namespace Widget {

class WidgetEvent;

class GUISystem {
private:
    Render::FontManager *fontManager;
    CompositeWidget *widgets;
    WidgetPoint screenSize;
public:
    GUISystem() : widgets(0) {}
    ~GUISystem();
    
    void construct();
    
    void render();
    void handleEvent(WidgetEvent *event);
    
    void setScreenSize(int width, int height);
};

}  // namespace Widget
}  // namespace Project

#endif
