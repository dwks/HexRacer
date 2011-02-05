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
    Widget::WidgetBase *currentScreen;
    Widget::WidgetPoint screenSize;
public:
    GUISystem() : widgets(0), currentScreen(0) {}
    ~GUISystem();
    
    void construct();
    
    void render();
    void handleEvent(Widget::WidgetEvent *event);
    
    void selectScreen(const std::string &screen);
    
    Widget::WidgetBase *getWidget(const std::string &path);
    
    void setScreenSize(Widget::WidgetPoint screenSize)
        { this->screenSize = screenSize; }
};

}  // namespace GUI
}  // namespace Project

#endif
