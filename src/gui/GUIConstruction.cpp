#include "GUIConstruction.h"

#include "boost/asio/ip/host_name.hpp"

#include "widget/CompositeWidget.h"
#include "widget/TextWidget.h"
#include "widget/ImageWidget.h"
#include "widget/ButtonWidget.h"
#include "widget/EditWidget.h"
#include "widget/CheckWidget.h"
#include "widget/ListWidget.h"

#include "widget/CentreUponChangeLayout.h"
#include "widget/SmoothTransitionLayout.h"
#include "widget/AbsoluteLayout.h"

#include "MapList.h"

#include "SDL_keysym.h"
#include "widget/KeyboardShortcutProxy.h"

#include "MainMenuProxy.h"
#include "SelectMapProxy.h"
#include "HostProxy.h"
#include "ConnectProxy.h"
#include "LobbyProxy.h"
#include "RunningProxy.h"
#include "PauseMenuProxy.h"
#include "SettingsProxy.h"
#include "LoadingProxy.h"
#include "SinglePlayerProxy.h"
#include "AboutProxy.h"
#include "FinishedProxy.h"

#include "misc/StreamAsString.h"
#include "settings/SettingsManager.h"
#include "map/Teams.h"

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
            finished        Race finished menu
            settings        The settings screen accessible from the main menu
    */
    
    constructMain();
    constructSelectMap();
    constructHost();
    constructConnect();
    constructLobby();
    constructSinglePlayer();
    constructLoading();  // must happen after host
    constructRunning();
    constructPaused();
    constructFinished();
    constructSettings();
    constructAbout();
    
    return widgets;
}

void GUIConstruction::constructMain() {
    Widget::CompositeWidget *main = new Widget::CompositeWidget("main");
    widgets->addChild(main);
    
    main->addChild(new Widget::ImageWidget("logo", "data/menu/hexracerlogo.png",
        Widget::WidgetRect(0.0, 0.0, 1.0, 1.0)));
    
    main->addChild(new Widget::ButtonWidget("host", "Host game",
        Widget::WidgetRect(-0.01, -0.002, 0.01, 0.002)));
    main->addChild(new Widget::ButtonWidget("join", "Join game",
        Widget::WidgetRect(-0.01, 0.4, 0.01, 0.002)));
    main->addChild(new Widget::ButtonWidget("single", "Practice",
        Widget::WidgetRect(-0.01, 0.8, 0.01, 0.002)));
    
    main->addChild(new Widget::ButtonWidget("settings", "Settings",
        Widget::WidgetRect(1.01, -0.002, 0.01, 0.002)));
    main->addChild(new Widget::ButtonWidget("about", "About",
        Widget::WidgetRect(1.01, 0.4, 0.01, 0.002)));
    main->addChild(new Widget::ButtonWidget("quit", "Quit",
        Widget::WidgetRect(1.01, 0.8, 0.01, 0.002)));
    
    static const int LOGO_DISPLAY = 500;
    smoothButtonUntil("main/logo", LOGO_DISPLAY, 800,
        Widget::WidgetRect(0.0, 0.05, 1.0, 0.5));
    
    smoothButtonUntil("main/host", LOGO_DISPLAY + 0, 500,
        Widget::WidgetRect(0.03, 0.65, 0.42, 0.08));
    smoothButtonUntil("main/join", LOGO_DISPLAY + 100, 450,
        Widget::WidgetRect(0.03, 0.75, 0.42, 0.08));
    smoothButtonUntil("main/single", LOGO_DISPLAY + 200, 400,
        Widget::WidgetRect(0.03, 0.85, 0.42, 0.08));
    
    smoothButtonUntil("main/settings", LOGO_DISPLAY + 150, 500,
        Widget::WidgetRect(0.55, 0.65, 0.42, 0.08));
    smoothButtonUntil("main/about", LOGO_DISPLAY + 250, 450,
        Widget::WidgetRect(0.55, 0.75, 0.42, 0.08));
    smoothButtonUntil("main/quit", LOGO_DISPLAY + 350, 400,
        Widget::WidgetRect(0.55, 0.85, 0.42, 0.08));
    
    setShortcut(getWidget("main/host"), SDLK_h);
    setShortcut(getWidget("main/join"), SDLK_j);
    setShortcut(getWidget("main/single"), SDLK_p);
    
    setShortcut(getWidget("main/settings"), SDLK_e);
    setShortcut(getWidget("main/about"), SDLK_a);
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
    
    host->addChild(new Widget::TextWidget("lapcount-label", "Number of laps:",
        Widget::NormalTextLayout::ALIGN_RIGHT,
        Widget::WidgetRect(0.1, 0.30, 0.35, 0.08)));
    host->addChild(new Widget::EditWidget("lapcount",
        GET_SETTING("game.race.laps", "3"),
        Widget::WidgetRect(0.5, 0.30, 0.35, 0.08)));
    
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
    getWidget("host/lapcount")->addEventProxy(proxy);
    
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

void GUIConstruction::constructLobby() {
    Widget::CompositeWidget *lobby
        = new Widget::CompositeWidget("lobby");
    widgets->addChild(lobby);
    
    std::string ip = boost::asio::ip::host_name();
    lobby->addChild(new Widget::TextWidget("ipaddress",
        Misc::StreamAsString() << "Server: " << ip, 0,
        Widget::WidgetRect(0.05, 0.02, 0.9, 0.07)));
    
    lobby->addChild(new Widget::TextWidget("player-label",
        "Player", 0,
        Widget::WidgetRect(0.05, 0.1, 0.43, 0.06)));
    lobby->addChild(new Widget::EditWidget("playername",
        Misc::StreamAsString() << "Anonymous",
        Widget::WidgetRect(0.05, 0.18, 0.43, 0.07)));
    
    Widget::ListWidget *colourList = new Widget::ListWidget("colourlist",
        true, false,
        Widget::WidgetRect(0.05, 0.26, 0.45, 0.2));
    lobby->addChild(colourList);
    
	for (int i = 0; i < Map::Teams::MAX_TEAMS; i++) {
		addListItem(colourList, Misc::StreamAsString() << i, Map::Teams::teamName(i), Map::Teams::teamColor(i));
	}
	/*
    addListItem(colourList, "0", "red", OpenGL::Color::RED);
    addListItem(colourList, "1", "green", OpenGL::Color::GREEN);
    addListItem(colourList, "2", "blue", OpenGL::Color::BLUE);
    addListItem(colourList, "3", "yellow", OpenGL::Color::YELLOW);
    addListItem(colourList, "4", "purple", OpenGL::Color::PURPLE);
    addListItem(colourList, "5", "pink", OpenGL::Color::PINK);
    addListItem(colourList, "6", "teal", OpenGL::Color::TEAL);
    addListItem(colourList, "7", "indigo", OpenGL::Color::INDIGO);
	*/
    
    lobby->addChild(new Widget::ListWidget("playerlist", true, false,
        Widget::WidgetRect(0.52, 0.1, 0.43, 0.36)));
    
    lobby->addChild(new Widget::ListWidget("chatlist", true, false,
        Widget::WidgetRect(0.05, 0.55, 0.9, 0.25)));
    lobby->addChild(new Widget::TextWidget("chat-label",
        "Chat:", Widget::NormalTextLayout::ALIGN_RIGHT,
        Widget::WidgetRect(0.05, 0.8, 0.1, 0.06)));
    lobby->addChild(new Widget::EditWidget("chat", "",
        Widget::WidgetRect(0.17, 0.8, 0.7, 0.06)));
    
    lobby->addChild(new Widget::ButtonWidget("cancel",
        "Disconnect", Widget::WidgetRect(0.1, 0.9, 0.35, 0.08)));
    lobby->addChild(new Widget::ButtonWidget("start",
        "Ready to start!", Widget::WidgetRect(0.5, 0.9, 0.4, 0.08)));
    
    setShortcut(getWidget("lobby/cancel"), SDLK_ESCAPE);
    
    boost::shared_ptr<Widget::EventProxy> proxy(new LobbyProxy(lobby));
    getWidget("lobby/ipaddress")->addEventProxy(proxy);
    getWidget("lobby/playername")->addEventProxy(proxy);
    getWidget("lobby/playerlist")->addEventProxy(proxy);
    
    getWidget("lobby/colourlist")->addEventProxy(proxy);
    
    getWidget("lobby/chatlist")->addEventProxy(proxy);
    getWidget("lobby/chat")->addEventProxy(proxy);
    
    getWidget("lobby/cancel")->addEventProxy(proxy);
    getWidget("lobby/start")->addEventProxy(proxy);
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

void GUIConstruction::constructFinished() {
    Widget::CompositeWidget *finished
        = new Widget::CompositeWidget("finished");
    widgets->addChild(finished);
    
    Widget::CompositeWidget *ranks = new Widget::CompositeWidget("ranks");
    ranks->setLayout(new Widget::AbsoluteLayout(Widget::WidgetRect(
        0.0, 0.0, 1.0, 1.0)));
    finished->addChild(ranks);
    
    Widget::CompositeWidget *teams = new Widget::CompositeWidget("teams");
    teams->setLayout(new Widget::AbsoluteLayout(Widget::WidgetRect(
        0.0, 0.0, 1.0, 1.0)));
    finished->addChild(teams);
    
    finished->addChild(new Widget::ButtonWidget("quit", "Quit",
        Widget::WidgetRect(0.3, 0.9, 0.4, 0.08)));
    
    setShortcut(getWidget("finished/quit"), SDLK_ESCAPE);
    setShortcut(getWidget("finished/quit"), SDLK_q);
    
    boost::shared_ptr<Widget::EventProxy> proxy(new FinishedProxy(finished));
    getWidget("finished")->addEventProxy(proxy);  // ensure proxy ref count >0
    getWidget("finished/quit")->addEventProxy(proxy);
}

void GUIConstruction::constructSettings() {
    Widget::CompositeWidget *settings
        = new Widget::CompositeWidget("settings");
    widgets->addChild(settings);
    
    settings->addChild(new Widget::TextWidget("screenmode-label",
        OpenGL::Color::WHITE, "Screen resolution:",
        Widget::NormalTextLayout::ALIGN_RIGHT));
    settings->getChild("screenmode-label")
        ->updateLayout(Widget::WidgetRect(0.1, 0.1, 0.35, 0.07));
    Misc::StreamAsString currentSize;
    currentSize
        << GET_SETTING("display.width", 0) << 'x'
        << GET_SETTING("display.height", 0);
    settings->addChild(new Widget::EditWidget("screenmode",
        currentSize, Widget::WidgetRect(0.5, 0.1, 0.35, 0.07)));
    
    settings->addChild(new Widget::CheckWidget("fullscreen",
        "Fullscreen", GET_SETTING("display.fullscreen", 0),
        Widget::WidgetRect(0.5, 0.2, 0.4, 0.07)));
    
    settings->addChild(new Widget::TextWidget("screenmode-label",
        "Rendering quality:", Widget::NormalTextLayout::ALIGN_RIGHT,
        Widget::WidgetRect(0.1, 0.35, 0.35, 0.07)));
    Widget::ListWidget *quality = new Widget::ListWidget(
        "quality", true, false,
        Widget::WidgetRect(0.5, 0.35, 0.3, 0.16));
    quality->addChild(new Widget::TextWidget("0", "Very low",
        Widget::NormalTextLayout::ALIGN_LEFT,
        Widget::WidgetRect(0.0, 0.0, 0.8, 0.035)));
    quality->addChild(new Widget::TextWidget("1", "Low",
        Widget::NormalTextLayout::ALIGN_LEFT,
        Widget::WidgetRect(0.0, 0.0, 0.8, 0.035)));
    quality->addChild(new Widget::TextWidget("2", "Medium",
        Widget::NormalTextLayout::ALIGN_LEFT,
        Widget::WidgetRect(0.0, 0.0, 0.8, 0.035)));
    quality->addChild(new Widget::TextWidget("3", "High",
        Widget::NormalTextLayout::ALIGN_LEFT,
        Widget::WidgetRect(0.0, 0.0, 0.8, 0.035)));
    settings->addChild(quality);
    
    settings->addChild(new Widget::TextWidget("renderingeffects-label",
        "Rendering effects:", Widget::NormalTextLayout::ALIGN_RIGHT,
        Widget::WidgetRect(0.1, 0.55, 0.35, 0.07)));
    settings->addChild(new Widget::CheckWidget("shadow",
        "Shadows", GET_SETTING("render.shadow.enable", 0),
        Widget::WidgetRect(0.5, 0.55, 0.4, 0.07)));
    settings->addChild(new Widget::CheckWidget("bloom",
        "Bloom", GET_SETTING("render.bloom.enable", 0),
        Widget::WidgetRect(0.5, 0.65, 0.4, 0.07)));
    
    settings->addChild(new Widget::ButtonWidget("joyselect",
        "Select joystick",
        Widget::WidgetRect(0.2, 0.77, 0.6, 0.08)));
    
    settings->addChild(new Widget::ButtonWidget("accept",
        "Accept settings", Widget::WidgetRect(0.3, 0.9, 0.4, 0.08)));
    
    setShortcut(getWidget("settings/accept"), SDLK_ESCAPE);
    
    boost::shared_ptr<Widget::EventProxy> proxy(new SettingsProxy(settings));
    getWidget("settings")->addEventProxy(proxy);
    getWidget("settings/screenmode")->addEventProxy(proxy);
    getWidget("settings/quality")->addEventProxy(proxy);
    getWidget("settings/shadow")->addEventProxy(proxy);
    getWidget("settings/bloom")->addEventProxy(proxy);
    getWidget("settings/fullscreen")->addEventProxy(proxy);
    getWidget("settings/joyselect")->addEventProxy(proxy);
    getWidget("settings/accept")->addEventProxy(proxy);
}

void GUIConstruction::constructAbout() {
    Widget::CompositeWidget *about
        = new Widget::CompositeWidget("about");
    widgets->addChild(about);
    
    about->addChild(new Widget::ButtonWidget("back", "Back to main menu",
        Widget::WidgetRect(0.2, 0.9, 0.6, 0.07)));
    
    boost::shared_ptr<Widget::EventProxy> proxy(new AboutProxy(about));
    getWidget("about/back")->addEventProxy(proxy);
}

void GUIConstruction::addListItem(Widget::ListWidget *list,
    const std::string &name, const std::string &text, OpenGL::Color color) {
    
    Widget::TextWidget *item = new Widget::TextWidget(name, text,
        Widget::NormalTextLayout::ALIGN_LEFT,
        Widget::WidgetRect(0.0, 0.0, 0.8, 0.04),
        color);
    
    list->addChild(item);
}

void GUIConstruction::smoothButtonUntil(const char *name,
    int start, int duration, const Widget::WidgetRect &destination) {
    
    Widget::WidgetBase *widget = getWidget(name);
    
    Widget::SmoothTransitionLayout *newLayout
        = new Widget::SmoothTransitionLayout(widget->getLayout(), duration,
            Widget::SmoothTransitionLayout::SMOOTH_END);
    widget->setLayout(newLayout);
    
    newLayout->doTransition(start, destination);
}

Widget::WidgetBase *GUIConstruction::getWidget(const std::string &path) {
    return widgets->getChildPath(path);
}

void GUIConstruction::setShortcut(Widget::WidgetBase *widget, long key) {
    widget->addEventProxy(new Widget::KeyboardShortcutProxy(widget, key));
}

}  // namespace GUI
}  // namespace Project
