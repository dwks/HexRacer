#ifndef PROJECT_GUI__GUISYSTEM_H
#define PROJECT_GUI__GUISYSTEM_H

#include "widget/CompositeWidget.h"
#include "widget/WidgetEvent.h"
#include "widget/FocusManager.h"

#include "render/FontManager.h"
#include "render/WidgetTextureList.h"

#include "event/Enabler.h"
#include "event/PauseGame.h"
#include "event/SwitchToScreen.h"

namespace Project {
namespace GUI {

class GUISystem : public Event::Enabler {
protected:
    void pauseGameHandler(Event::PauseGame *event);
    void switchToScreenHandler(Event::SwitchToScreen *event);
private:
    Render::FontManager *fontManager;
    Render::WidgetTextureList *textureList;
    Widget::FocusManager *focusManager;
    
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
    Widget::CompositeWidget *getScreen(const std::string &path);
    Widget::WidgetBase *getCurrentScreen() { return currentScreen; }
    
    void setScreenSize(Widget::WidgetPoint screenSize)
        { this->screenSize = screenSize; }
};

}  // namespace GUI
}  // namespace Project

#endif
