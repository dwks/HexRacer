#include "GUISystem.h"
#include "MainMenuProxy.h"
#include "HostProxy.h"
#include "ConnectProxy.h"
#include "RunningProxy.h"
#include "PauseMenuProxy.h"
#include "SettingsProxy.h"

#include "MapList.h"

#include "widget/WidgetRenderer.h"
#include "widget/ButtonWidget.h"
#include "widget/EditWidget.h"
#include "widget/ListWidget.h"
#include "widget/ImageWidget.h"

#include "widget/NormalTextLayout.h"

#include "widget/KeyEvent.h"
#include "widget/MouseMoveEvent.h"
#include "widget/MouseButtonEvent.h"

#include "widget/KeyboardShortcutProxy.h"

#include "event/EventSystem.h"

#include "SDL.h"
#include "SDL_events.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace GUI {

void GUISystem::pauseGameHandler(Event::PauseGame *event) {
    if(event->getPaused()) {
        selectScreen("paused");
    }
    else {
        selectScreen("running");
    }
}

void GUISystem::switchToScreenHandler(Event::SwitchToScreen *event) {
    selectScreen(event->getScreen());
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
    
    widgets = new Widget::CompositeWidget("gui");
    
    /*
        Screens:
            main            Main menu
            host            Hosting screen with map selection
            connect         Screen which prompts for host/port
            running         Displayed while the game is running
            paused          Paused menu
            settings        The settings screen accessible from the main menu
    */
    
    {
        Widget::CompositeWidget *main
            = new Widget::CompositeWidget("main");
        widgets->addChild(main);

		main->addChild(new Widget::ImageWidget("logo", "data/menu/hexracerlogo.png",
			Widget::WidgetRect(0.0, 0.05, 1.0, 0.5)));
        
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
        
        setShortcut(getWidget("main/host"), SDLK_h);
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
        Widget::CompositeWidget *host
            = new Widget::CompositeWidget("host");
        widgets->addChild(host);
        
        /*host->addChild(new Widget::ScrollbarWidget("scrollbar-horizontal", false,
            Widget::WidgetRect(0.1, 0.4, 0.8, 0.08)));
        host->addChild(new Widget::ScrollbarWidget("scrollbar-vertical", true,
            Widget::WidgetRect(0.7, 0.1, 0.05, 0.3)));*/
        
        Widget::ListWidget *mapList = new MapList(
            "maplist", true, false,
            Widget::WidgetRect(0.1, 0.1, 0.8, 0.3));
        host->addChild(mapList);
        
        host->addChild(new Widget::TextWidget("title", "",
            Widget::NormalTextLayout::ALIGN_HCENTRE,
            Widget::WidgetRect(0.1, 0.42, 0.8, 0.07)));
        
        host->addChild(new Widget::ImageWidget("thumbnail", "",
            Widget::WidgetRect(0.2, 0.5, 0.6, 0.3)));
        
        host->addChild(new Widget::ButtonWidget("cancel",
            "Cancel", Widget::WidgetRect(0.1, 0.85, 0.35, 0.08)));
        host->addChild(new Widget::ButtonWidget("host",
            "Host game", Widget::WidgetRect(0.5, 0.85, 0.35, 0.08)));
        
        setShortcut(getWidget("host/cancel"), SDLK_ESCAPE);
        setShortcut(getWidget("host/host"), SDLK_RETURN);
        
        getWidget("host/maplist")->addEventProxy(new HostProxy(host));
        getWidget("host/cancel")->addEventProxy(new HostProxy(host));
        getWidget("host/host")->addEventProxy(new HostProxy(host));
    }
    
    {
        Widget::CompositeWidget *connect
            = new Widget::CompositeWidget("connect");
        widgets->addChild(connect);
        
        connect->addChild(new Widget::TextWidget("host-label", "Host:",
            Widget::NormalTextLayout::ALIGN_RIGHT | Widget::NormalTextLayout::ALIGN_VCENTRE,
            Widget::WidgetRect(0.1, 0.3, 0.35, 0.08)));
        connect->addChild(new Widget::EditWidget("host",
            GET_SETTING("network.host", "localhost"),
            Widget::WidgetRect(0.5, 0.3, 0.35, 0.08)));
        
        connect->addChild(new Widget::TextWidget("port-label", "Port:",
            Widget::NormalTextLayout::ALIGN_RIGHT | Widget::NormalTextLayout::ALIGN_VCENTRE,
            Widget::WidgetRect(0.1, 0.4, 0.35, 0.08)));
        connect->addChild(new Widget::EditWidget("port",
            GET_SETTING("network.port", "1820"),
            Widget::WidgetRect(0.5, 0.4, 0.35, 0.08)));
        
        connect->addChild(new Widget::ButtonWidget("cancel",
            "Cancel", Widget::WidgetRect(0.1, 0.5, 0.35, 0.08)));
        connect->addChild(new Widget::ButtonWidget("connect",
            "Connect", Widget::WidgetRect(0.5, 0.5, 0.35, 0.08)));
        
        connect->addChild(new Widget::TextWidget("error", "",
            Widget::NormalTextLayout::ALIGN_HCENTRE | Widget::NormalTextLayout::ALIGN_VCENTRE,
            Widget::WidgetRect(0.1, 0.6, 0.8, 0.08)));
        
        setShortcut(getWidget("connect/cancel"), SDLK_ESCAPE);
        setShortcut(getWidget("connect/connect"), SDLK_RETURN);
        
        getWidget("connect/host")->addEventProxy(new ConnectProxy(connect));
        getWidget("connect/port")->addEventProxy(new ConnectProxy(connect));
        
        getWidget("connect/cancel")->addEventProxy(new ConnectProxy(connect));
        getWidget("connect/connect")->addEventProxy(new ConnectProxy(connect));
    }
    
    {
        Widget::CompositeWidget *running
            = new Widget::CompositeWidget("running");
        widgets->addChild(running);
        
        /*running->addChild(new Widget::ButtonWidget("menu",
            "Menu", Widget::WidgetRect(0.0, 0.0, 0.1, 0.05)));
        
        setShortcut(getWidget("running/menu"), SDLK_ESCAPE);*/
        
        running->addChild(new Widget::TextWidget(
            "lapcount", "Lap 1", 0,
            Widget::WidgetRect(0.0, 0.0, 0.2, 0.06)));
        
        getWidget("running")->addEventProxy(new RunningProxy(running));
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
        
        settings->addChild(new Widget::TextWidget("screenmode-label",
            OpenGL::Color::WHITE, "Screen resolution:",
            Widget::NormalTextLayout::ALIGN_RIGHT | Widget::NormalTextLayout::ALIGN_VCENTRE));
        settings->getChild("screenmode-label")
            ->updateLayout(Widget::WidgetRect(0.1, 0.1, 0.35, 0.08));
        Misc::StreamAsString currentSize;
        currentSize
            << GET_SETTING("display.width", 0) << 'x'
            << GET_SETTING("display.height", 0);
        settings->addChild(new Widget::EditWidget("screenmode",
            currentSize, Widget::WidgetRect(0.5, 0.1, 0.35, 0.08)));
        
        settings->addChild(new Widget::EditWidget("testing",
            "test", Widget::WidgetRect(0.5, 0.2, 0.35, 0.08)));
        
        settings->addChild(new Widget::ButtonWidget("accept",
            "Accept settings", Widget::WidgetRect(0.3, 0.9, 0.4, 0.08)));
        
        setShortcut(getWidget("settings/accept"), SDLK_ESCAPE);
        
        getWidget("settings/screenmode")->addEventProxy(new SettingsProxy());
        getWidget("settings/accept")->addEventProxy(new SettingsProxy());
    }
    
    selectScreen("main");
    
    METHOD_OBSERVER(&GUISystem::pauseGameHandler);
    METHOD_OBSERVER(&GUISystem::switchToScreenHandler);
}

void GUISystem::render() {
    Widget::WidgetRenderer renderer(screenSize);
    
    renderer.begin();
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

void GUISystem::selectScreen(const std::string &screen) {
    currentScreen = getWidget(screen);
    
    // hack to enable unicode translation for screens with edit widgets
    if(screen == "connect" || screen == "settings") {
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
