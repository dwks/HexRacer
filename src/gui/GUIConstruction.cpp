#include "GUIConstruction.h"

#include "widget/CompositeWidget.h"
#include "widget/TextWidget.h"
#include "widget/ImageWidget.h"
#include "widget/ButtonWidget.h"
#include "widget/EditWidget.h"
#include "widget/CheckWidget.h"

#include "widget/CentreUponChangeLayout.h"

#include "MapList.h"

#include "SDL_keysym.h"
#include "widget/KeyboardShortcutProxy.h"

#include "MainMenuProxy.h"
#include "SelectMapProxy.h"
#include "HostProxy.h"
#include "ConnectProxy.h"
#include "RunningProxy.h"
#include "PauseMenuProxy.h"
#include "SettingsProxy.h"
#include "LoadingProxy.h"
#include "SinglePlayerProxy.h"

#include "misc/StreamAsString.h"
#include "settings/SettingsManager.h"

namespace Project {
namespace GUI {

Widget::CompositeWidget *GUIConstruction::construct() {
    widgets = new Widget::CompositeWidget("gui");
    
    /*
        Screens:
            main            Main menu
            selectmap       Map selection screen
            host            Multiplayer game hosting screen
            connect         Screen which prompts for host/port
            singleplayer    Begins a single-player game
            loading         Loading screen
            running         Displayed while the game is running
            paused          Paused menu
            settings        The settings screen accessible from the main menu
    */
    
    constructMain();
    constructSelectMap();
    constructHost();
    constructConnect();
    constructSinglePlayer();
    constructLoading();  // must happen after host
    constructRunning();
    constructPaused();
    constructSettings();
    
    return widgets;
}

void GUIConstruction::constructMain() {
    Widget::CompositeWidget *main = new Widget::CompositeWidget("main");
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
    
    boost::shared_ptr<Widget::EventProxy> proxy(new MainMenuProxy());
    getWidget("main/host")->addEventProxy(proxy);
    getWidget("main/join")->addEventProxy(proxy);
    getWidget("main/single")->addEventProxy(proxy);
    
    getWidget("main/settings")->addEventProxy(proxy);
    getWidget("main/about")->addEventProxy(proxy);
    getWidget("main/quit")->addEventProxy(proxy);
}

void GUIConstruction::constructSelectMap() {
    Widget::CompositeWidget *selectmap
        = new Widget::CompositeWidget("selectmap");
    widgets->addChild(selectmap);
    
    Widget::ListWidget *mapList = new MapList(
        "maplist", true, false,
        Widget::WidgetRect(0.1, 0.1, 0.8, 0.3));
    selectmap->addChild(mapList);
    
    selectmap->addChild(new Widget::TextWidget("title", "",
        Widget::NormalTextLayout::ALIGN_HCENTRE,
        Widget::WidgetRect(0.1, 0.42, 0.8, 0.07)));
    
    selectmap->addChild(new Widget::ImageWidget("thumbnail", "",
        Widget::WidgetRect(0.2, 0.5, 0.6, 0.3)));
    
    selectmap->addChild(new Widget::ButtonWidget("selectmap",
        "Use selected map", Widget::WidgetRect(0.2, 0.85, 0.6, 0.08)));
    
    setShortcut(getWidget("selectmap/selectmap"), SDLK_ESCAPE);
    setShortcut(getWidget("selectmap/selectmap"), SDLK_RETURN);
    
    boost::shared_ptr<Widget::EventProxy> proxy(new SelectMapProxy(selectmap));
    getWidget("selectmap/maplist")->addEventProxy(proxy);
    getWidget("selectmap/selectmap")->addEventProxy(proxy);
}

void GUIConstruction::constructHost() {
    Widget::CompositeWidget *host = new Widget::CompositeWidget("host");
    widgets->addChild(host);
    
    host->addChild(new Widget::ButtonWidget("map",
        "Choose map", Widget::WidgetRect(0.1, 0.1, 0.4, 0.08)));
    
    host->addChild(new Widget::TextWidget("aicount-label", "Number of AIs:",
        Widget::NormalTextLayout::ALIGN_RIGHT,
        Widget::WidgetRect(0.1, 0.20, 0.35, 0.08)));
    host->addChild(new Widget::EditWidget("aicount",
        GET_SETTING("server.aicount", "0"),
        Widget::WidgetRect(0.5, 0.20, 0.35, 0.08)));
    
    host->addChild(new Widget::TextWidget("hostport-label", "Host port:",
        Widget::NormalTextLayout::ALIGN_RIGHT,
        Widget::WidgetRect(0.1, 0.60, 0.35, 0.08)));
    host->addChild(new Widget::EditWidget("hostport",
        GET_SETTING("network.serverport", "1820"),
        Widget::WidgetRect(0.5, 0.60, 0.35, 0.08)));
    
    host->addChild(new Widget::TextWidget("error", "", 0,
        Widget::WidgetRect(0.1, 0.7, 0.8, 0.08)));
    
    host->addChild(new Widget::ButtonWidget("cancel",
        "Cancel", Widget::WidgetRect(0.1, 0.85, 0.35, 0.08)));
    host->addChild(new Widget::ButtonWidget("host",
        "Host game", Widget::WidgetRect(0.5, 0.85, 0.35, 0.08)));
    
    setShortcut(getWidget("host/map"), SDLK_m);
    setShortcut(getWidget("host/host"), SDLK_h);
    
    setShortcut(getWidget("host/cancel"), SDLK_ESCAPE);
    setShortcut(getWidget("host/host"), SDLK_RETURN);
    
    boost::shared_ptr<Widget::EventProxy> proxy(new HostProxy(host));
    getWidget("host/map")->addEventProxy(proxy);
    getWidget("host/aicount")->addEventProxy(proxy);
    
    getWidget("host/hostport")->addEventProxy(proxy);
    
    getWidget("host/cancel")->addEventProxy(proxy);
    getWidget("host/host")->addEventProxy(proxy);
}

void GUIConstruction::constructSinglePlayer() {
    Widget::CompositeWidget *singleplayer
        = new Widget::CompositeWidget("singleplayer");
    widgets->addChild(singleplayer);
    
    singleplayer->addChild(new Widget::ButtonWidget("map",
        "Choose map", Widget::WidgetRect(0.1, 0.1, 0.4, 0.08)));
    
    singleplayer->addChild(new Widget::ButtonWidget("cancel",
        "Cancel", Widget::WidgetRect(0.1, 0.85, 0.35, 0.08)));
    singleplayer->addChild(new Widget::ButtonWidget("start",
        "Start game", Widget::WidgetRect(0.5, 0.85, 0.35, 0.08)));
    
    setShortcut(getWidget("singleplayer/map"), SDLK_m);
    setShortcut(getWidget("singleplayer/start"), SDLK_s);
    
    setShortcut(getWidget("singleplayer/cancel"), SDLK_ESCAPE);
    setShortcut(getWidget("singleplayer/start"), SDLK_RETURN);
    
    boost::shared_ptr<Widget::EventProxy> proxy(
        new SinglePlayerProxy(singleplayer));
    getWidget("singleplayer/map")->addEventProxy(proxy);
    getWidget("singleplayer/cancel")->addEventProxy(proxy);
    getWidget("singleplayer/start")->addEventProxy(proxy);
}

void GUIConstruction::constructConnect() {
    Widget::CompositeWidget *connect = new Widget::CompositeWidget("connect");
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
    
    boost::shared_ptr<Widget::EventProxy> proxy(new ConnectProxy(connect));
    getWidget("connect/host")->addEventProxy(proxy);
    getWidget("connect/port")->addEventProxy(proxy);
    
    getWidget("connect/cancel")->addEventProxy(proxy);
    getWidget("connect/connect")->addEventProxy(proxy);
}

void GUIConstruction::constructLoading() {
    Widget::CompositeWidget *loading
        = new Widget::CompositeWidget("loading");
    widgets->addChild(loading);
    
    loading->addChild(new Widget::ImageWidget("thumbnail", "",
        Widget::WidgetRect(0.1, 0.1, 0.8, 0.5)));
    loading->addChild(new Widget::TextWidget("loading", "Loading ...", 0,
        Widget::WidgetRect(0.1, 0.65, 0.8, 0.3)));
    
    boost::shared_ptr<Widget::EventProxy> proxy(
        new LoadingProxy(loading, getWidget("host")));
    getWidget("loading")->addEventProxy(proxy);
}

void GUIConstruction::constructRunning() {
    Widget::CompositeWidget *running
        = new Widget::CompositeWidget("running");
    widgets->addChild(running);
    
    Widget::WidgetBase *lapcount = new Widget::TextWidget(
        "lapcount", "Lap 1", 0,
        Widget::WidgetRect(0.0, 0.0, 0.2, 0.06));
    lapcount->setLayout(new Widget::CentreUponChangeLayout(
        lapcount->getLayout(), 1500, 500,
        Widget::WidgetRect(0.2, 0.3, 0.6, 0.2)));
    running->addChild(lapcount);
    
    boost::shared_ptr<Widget::EventProxy> proxy(new RunningProxy(running));
    getWidget("running")->addEventProxy(proxy);
}

void GUIConstruction::constructPaused() {
    Widget::CompositeWidget *paused
        = new Widget::CompositeWidget("paused");
    widgets->addChild(paused);
    
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
    
    boost::shared_ptr<Widget::EventProxy> proxy(new PauseMenuProxy());
    getWidget("paused/resume")->addEventProxy(proxy);
    getWidget("paused/settings")->addEventProxy(proxy);
    getWidget("paused/quit")->addEventProxy(proxy);
}

void GUIConstruction::constructSettings() {
    Widget::CompositeWidget *settings
        = new Widget::CompositeWidget("settings");
    widgets->addChild(settings);
    
    settings->addChild(new Widget::TextWidget("screenmode-label",
        OpenGL::Color::WHITE, "Screen resolution:",
        Widget::NormalTextLayout::ALIGN_RIGHT));
    settings->getChild("screenmode-label")
        ->updateLayout(Widget::WidgetRect(0.1, 0.1, 0.35, 0.08));
    Misc::StreamAsString currentSize;
    currentSize
        << GET_SETTING("display.width", 0) << 'x'
        << GET_SETTING("display.height", 0);
    settings->addChild(new Widget::EditWidget("screenmode",
        currentSize, Widget::WidgetRect(0.5, 0.1, 0.35, 0.08)));
    
    settings->addChild(new Widget::CheckWidget("fullscreen",
        "Fullscreen", GET_SETTING("display.fullscreen", 0),
        Widget::WidgetRect(0.5, 0.2, 0.35, 0.08)));
    
    settings->addChild(new Widget::ButtonWidget("accept",
        "Accept settings", Widget::WidgetRect(0.3, 0.9, 0.4, 0.08)));
    
    setShortcut(getWidget("settings/accept"), SDLK_ESCAPE);
    
    boost::shared_ptr<Widget::EventProxy> proxy(new SettingsProxy());
    getWidget("settings/screenmode")->addEventProxy(proxy);
    getWidget("settings/fullscreen")->addEventProxy(proxy);
    getWidget("settings/accept")->addEventProxy(proxy);
}

Widget::WidgetBase *GUIConstruction::getWidget(const std::string &path) {
    return widgets->getChildPath(path);
}

void GUIConstruction::setShortcut(Widget::WidgetBase *widget, long key) {
    widget->addEventProxy(new Widget::KeyboardShortcutProxy(widget, key));
}

}  // namespace GUI
}  // namespace Project
