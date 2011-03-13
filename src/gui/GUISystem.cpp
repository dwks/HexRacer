#include "GUISystem.h"
#include "GUIConstruction.h"

#include "widget/WidgetRenderer.h"

#include "widget/KeyEvent.h"
#include "widget/MouseMoveEvent.h"
#include "widget/MouseButtonEvent.h"

#include "event/EventSystem.h"

#include "SDL.h"

#include "log/Logger.h"

namespace Project {
namespace GUI {

void GUISystem::pauseGameHandler(Event::PauseGame *event) {
    if(event->getPaused()) {
        pushScreen("paused");
    }
    else {
        popScreen("running");
    }
}

void GUISystem::switchToScreenHandler(Event::SwitchToScreen *event) {
    if(event->getScreen().empty()) {
        LOG(GUI, "popping current screen");
        popScreen();
    }
    else if(event->getScreen()[0] == '-') {
        std::string until = event->getScreen().substr(1);
        LOG(GUI, "popping until screen " << until);
        popScreen(until);
    }
    else {
        LOG(GUI, "pushing screen \"" << event->getScreen() << "\"");
        pushScreen(event->getScreen());
    }
}

GUISystem::~GUISystem() {
    LOG(GUI, "Destructing GUI system");
    
    delete widgets;
    
    delete textureList;
    delete fontManager;
    delete focusManager;
}

void GUISystem::construct() {
    fontManager = new Render::FontManager();
    textureList = new Render::WidgetTextureList();
    focusManager = new Widget::FocusManager();
    
    LOG(GUI, "Constructing GUI system");
    widgets = GUIConstruction().construct();
    
    pushScreen("main");
    
    METHOD_OBSERVER(&GUISystem::pauseGameHandler);
    METHOD_OBSERVER(&GUISystem::switchToScreenHandler);
}

void GUISystem::render() {
    Widget::WidgetRenderer renderer(screenSize);
    
    renderer.begin();
    
    // first do actual rendering
    currentScreen->accept(renderer);
    
    renderer.end();
}

void GUISystem::handleEvent(Widget::WidgetEvent *event) {
    if(dynamic_cast<Widget::KeyEvent *>(event)
        && focusManager->getKeyFocus()) {
        
        focusManager->getKeyFocus()->handleEvent(event);
    }
    else if(dynamic_cast<Widget::MouseMoveEvent *>(event)
        && focusManager->getClickFocus()) {
        
        focusManager->getClickFocus()->handleEvent(event);
    }
    else {
        currentScreen->handleEvent(event);
    }
    
    if(dynamic_cast<Widget::MouseButtonEvent *>(event)) {
        Widget::MouseButtonEvent *button
            = dynamic_cast<Widget::MouseButtonEvent *>(event);
        if(button->getButton() == Widget::MouseButtonEvent::BUTTON_LEFT
            && !button->getDown()) {
            
            focusManager->setClickFocus(NULL);
        }
        
        if(button->getButton() == Widget::MouseButtonEvent::BUTTON_LEFT
            && button->getDown()) {
            
            Widget::WidgetBase *keyFocus = focusManager->getKeyFocus();
            Widget::WidgetPoint where = button->getWhere();
            if(keyFocus && !keyFocus->getBoundingRect().pointInside(where)) {
                focusManager->setKeyFocus(NULL);
            }
        }
    }
}

void GUISystem::pushScreen(const std::string &screen) {
    screenStack.push_back(screen);
    useScreen(screen);
}

void GUISystem::popScreen() {
    if(screenStack.size() <= 1) return;
    
    screenStack.pop_back();
    
    useScreen(screenStack.back());
}

void GUISystem::popScreen(const std::string &until) {
    while(!screenStack.empty() && screenStack.back() != until) {
        screenStack.pop_back();
    }
    
    if(screenStack.empty()) screenStack.push_back("main");
    
    useScreen(screenStack.back());
}

void GUISystem::useScreen(const std::string &screen) {
    currentScreen = getWidget(screen);
    
    // hack to enable unicode translation for screens with edit widgets
    if(screen != "running") {
        SDL_EnableUNICODE(1);
        SDL_EnableKeyRepeat(
            SDL_DEFAULT_REPEAT_DELAY,
            SDL_DEFAULT_REPEAT_INTERVAL);
    }
    else {
        SDL_EnableUNICODE(0);
        SDL_EnableKeyRepeat(0, 0);
    }
}

Widget::WidgetBase *GUISystem::getWidget(const std::string &path) {
    return widgets->getChildPath(path);
}

Widget::CompositeWidget *GUISystem::getScreen(const std::string &path) {
    return dynamic_cast<Widget::CompositeWidget *>(getWidget(path));
}

}  // namespace GUI
}  // namespace Project
