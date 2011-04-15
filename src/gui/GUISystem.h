#ifndef PROJECT_GUI__GUISYSTEM_H
#define PROJECT_GUI__GUISYSTEM_H

#include <string>
#include <vector>

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
private:
    std::vector<std::string> screenStack;
public:
    GUISystem() : widgets(0), currentScreen(0) {}
    ~GUISystem();
    
    void construct();
    
    void render(bool inMenu);
    void handleEvent(Widget::WidgetEvent *event);
    
    /** Pushes one screen onto the screen stack, which will become the current
        screen.
    */
    void pushScreen(const std::string &screen);
    
    /** Pops one screen from the screen stack.
    */
    void popScreen();
    
    /** Pops as many screens as necessary from the screen stack until @a until
        is on top.
    */
    void popScreen(const std::string &until);
    
    Widget::WidgetBase *getWidget(const std::string &path);
    Widget::CompositeWidget *getScreen(const std::string &path);
    Widget::WidgetBase *getCurrentScreen() { return currentScreen; }
    
    void setScreenSize(Widget::WidgetPoint screenSize)
        { this->screenSize = screenSize; }
private:
    void useScreen(const std::string &screen);
};

}  // namespace GUI
}  // namespace Project

#endif
