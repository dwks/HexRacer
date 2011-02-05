#ifndef PROJECT_GUI__GUISYSTEM_H
#define PROJECT_GUI__GUISYSTEM_H

#include "widget/CompositeWidget.h"
#include "widget/WidgetEvent.h"
#include "render/FontManager.h"

namespace Project {
namespace GUI {

class GUISystem {
private:
    Render::FontManager *fontManager;
    Widget::CompositeWidget *widgets;
    Widget::WidgetPoint screenSize;
public:
    GUISystem() : widgets(0) {}
    ~GUISystem();
    
    void construct();
    
    void render();
    void handleEvent(Widget::WidgetEvent *event);
    
    Widget::WidgetBase *getWidget(const std::string &path);
};

}  // namespace GUI
}  // namespace Project

#endif
