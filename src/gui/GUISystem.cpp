#include "GUISystem.h"
#include "MainMenuProxy.h"
#include "RunningProxy.h"
#include "PauseMenuProxy.h"
#include "SettingsProxy.h"

#include "widget/WidgetRenderer.h"
#include "widget/ButtonWidget.h"
#include "widget/EditWidget.h"
#include "widget/NormalTextLayout.h"
#include "widget/MouseButtonEvent.h"
#include "widget/KeyboardShortcutProxy.h"

#include "GUIObservers.h"

#include "SDL.h"
#include "SDL_events.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

GUISystem::~GUISystem() {
    delete widgets;
    delete textureList;
    delete fontManager;
    delete observers;
}

void GUISystem::construct() {
    fontManager = new Render::FontManager();
    textureList = new Render::WidgetTextureList();
    focusManager = new Widget::FocusManager();
    
    widgets = new Widget::CompositeWidget("gui");
    
    {
        Widget::CompositeWidget *main
            = new Widget::CompositeWidget("main");
        widgets->addChild(main);
        
        main->addChild(new Widget::TextWidget("title", OpenGL::Color::WHITE,
            "HexRacer",
            Widget::NormalTextLayout::ALIGN_HCENTRE | Widget::NormalTextLayout::ALIGN_TOP));
        
        main->getChild("title")->updateLayout(
            Widget::WidgetRect(0.0, 0.1, 1.0, 0.2));
        
        main->addChild(new Widget::ButtonWidget("host", "Host game",
            Widget::WidgetRect(0.03, 0.6, 0.42, 0.08)));
        main->addChild(new Widget::ButtonWidget("join", "Join game",
            Widget::WidgetRect(0.03, 0.7, 0.42, 0.08)));
        main->addChild(new Widget::ButtonWidget("single", "Single-player",
            Widget::WidgetRect(0.03, 0.8, 0.42, 0.08)));
        
        main->addChild(new Widget::ButtonWidget("settings", "Settings",
            Widget::WidgetRect(0.55, 0.6, 0.42, 0.08)));
        main->addChild(new Widget::ButtonWidget("about", "About",
            Widget::WidgetRect(0.55, 0.7, 0.42, 0.08)));
        main->addChild(new Widget::ButtonWidget("quit", "Quit",
            Widget::WidgetRect(0.55, 0.8, 0.42, 0.08)));
        
        setShortcut(getWidget("main/join"), SDLK_j);
        setShortcut(getWidget("main/single"), SDLK_s);
        
        setShortcut(getWidget("main/quit"), SDLK_q);
        setShortcut(getWidget("main/quit"), SDLK_ESCAPE);
        
        getWidget("main/host")->addEventProxy(new MainMenuProxy());
        getWidget("main/join")->addEventProxy(new MainMenuProxy());
        getWidget("main/single")->addEventProxy(new MainMenuProxy());
        
        getWidget("main/settings")->addEventProxy(new MainMenuProxy());
        getWidget("main/about")->addEventProxy(new MainMenuProxy());
        getWidget("main/quit")->addEventProxy(new MainMenuProxy());
    }
    
    {
        Widget::CompositeWidget *running
            = new Widget::CompositeWidget("running");
        widgets->addChild(running);
        
        running->addChild(new Widget::ButtonWidget("menu",
            "Menu", Widget::WidgetRect(0.0, 0.0, 0.1, 0.05)));
        
        setShortcut(getWidget("running/menu"), SDLK_ESCAPE);
        
        getWidget("running/menu")->addEventProxy(new RunningProxy());
    }
    
    {
        Widget::CompositeWidget *paused
            = new Widget::CompositeWidget("paused");
        widgets->addChild(paused);
        
        /*paused->addChild(new Widget::ButtonWidget("help",
            "?", Widget::WidgetRect(0.0, 0.0, 0.1, 0.1)));*/
        
        paused->addChild(new Widget::ButtonWidget("resume", "Resume",
            Widget::WidgetRect(0.3, 0.4 - 0.15, 0.4, 0.1)));
        paused->addChild(new Widget::ButtonWidget("settings", "Settings",
            Widget::WidgetRect(0.3, 0.4, 0.4, 0.1)));
        paused->addChild(new Widget::ButtonWidget("quit", "Quit",
            Widget::WidgetRect(0.3, 0.4 + 0.15, 0.4, 0.1)));
        
        setShortcut(getWidget("paused/resume"), SDLK_ESCAPE);
        setShortcut(getWidget("paused/resume"), SDLK_r);
        setShortcut(getWidget("paused/settings"), SDLK_s);
        setShortcut(getWidget("paused/quit"), SDLK_q);
        
        getWidget("paused/resume")->addEventProxy(new PauseMenuProxy());
        getWidget("paused/settings")->addEventProxy(new PauseMenuProxy());
        getWidget("paused/quit")->addEventProxy(new PauseMenuProxy());
    }
    
    {
        Widget::CompositeWidget *settings
            = new Widget::CompositeWidget("settings");
        widgets->addChild(settings);
        
        Misc::StreamAsString currentSize;
        currentSize
            << GET_SETTING("display.width", 0) << 'x'
            << GET_SETTING("display.height", 0);
        settings->addChild(new Widget::EditWidget("screenmode",
            currentSize, Widget::WidgetRect(0.1, 0.1, 0.35, 0.08)));
        
        settings->addChild(new Widget::ButtonWidget("accept",
            "Accept settings", Widget::WidgetRect(0.3, 0.9, 0.4, 0.08)));
        
        setShortcut(getWidget("settings/accept"), SDLK_ESCAPE);
        
        getWidget("settings/accept")->addEventProxy(new SettingsProxy());
    }
    
    selectScreen("main");
    
    observers = new GUIObservers(this);
}

void GUISystem::render() {
    Widget::WidgetRenderer renderer(screenSize);
    
    renderer.begin();
    currentScreen->accept(renderer);
    renderer.end();
}

void GUISystem::handleEvent(Widget::WidgetEvent *event) {
    currentScreen->handleEvent(event);
    
    if(dynamic_cast<Widget::MouseButtonEvent *>(event)) {
        Widget::MouseButtonEvent *button
            = dynamic_cast<Widget::MouseButtonEvent *>(event);
        if(button->getButton() == Widget::MouseButtonEvent::BUTTON_LEFT
            && !button->getDown()) {
            
            focusManager->setClickFocus(NULL);
        }
    }
}

void GUISystem::selectScreen(const std::string &screen) {
    currentScreen = getWidget(screen);
    
    // hack to enable unicode translation for screens with edit widgets
    if(screen == "settings") {
        SDL_EnableUNICODE(1);
    }
    else {
        SDL_EnableUNICODE(0);
    }
}

Widget::WidgetBase *GUISystem::getWidget(const std::string &path) {
    Widget::WidgetBase *widget = this->widgets;
    
    std::string::size_type start = 0, end;
    do {
        end = path.find('/', start);
        
        widget = widget->getChild(path.substr(start, end));
        
        start = end + 1;
    } while(widget && end != std::string::npos);
    
    return widget;
}

Widget::CompositeWidget *GUISystem::getScreen(const std::string &path) {
    return dynamic_cast<Widget::CompositeWidget *>(getWidget(path));
}

void GUISystem::setShortcut(Widget::WidgetBase *widget, long key) {
    widget->addEventProxy(new Widget::KeyboardShortcutProxy(widget, key));
}

}  // namespace GUI
}  // namespace Project
