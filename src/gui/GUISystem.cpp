#include "GUISystem.h"
#include "PauseMenuProxy.h"

#include "widget/WidgetRenderer.h"
#include "widget/ButtonWidget.h"

namespace Project {
namespace GUI {

GUISystem::~GUISystem() {
    delete widgets;
    delete fontManager;
}

void GUISystem::construct() {
    fontManager = new Render::FontManager();
    
    widgets = new Widget::CompositeWidget("gui");
    
    widgets->addChild(new Widget::ButtonWidget("help",
        "?", Widget::WidgetRect(0.0, 0.0, 0.1, 0.1)));
    
    widgets->addChild(new Widget::ButtonWidget("resume",
        "Resume", Widget::WidgetRect(0.3, 0.4 - 0.15, 0.4, 0.1)));
    
    widgets->addChild(new Widget::ButtonWidget("settings",
        "Settings", Widget::WidgetRect(0.3, 0.4, 0.4, 0.1)));
    
    widgets->addChild(new Widget::ButtonWidget("quit",
        "Quit", Widget::WidgetRect(0.3, 0.4 + 0.15, 0.4, 0.1)));
    
    widgets->getChild("help")->addEventProxy(new PauseMenuProxy());
    widgets->getChild("resume")->addEventProxy(new PauseMenuProxy());
    widgets->getChild("settings")->addEventProxy(new PauseMenuProxy());
    widgets->getChild("quit")->addEventProxy(new PauseMenuProxy());
}

void GUISystem::render() {
    Widget::WidgetRenderer renderer(screenSize);
    
    renderer.begin();
    widgets->accept(renderer);
    renderer.end();
}

void GUISystem::handleEvent(Widget::WidgetEvent *event) {
    widgets->handleEvent(event);
}

Widget::WidgetBase *GUISystem::getWidget(const std::string &path) {
    Widget::WidgetBase *widget = this->widgets;
    
    std::string::size_type start = 0, end;
    do {
        end = path.find('/');
        
        widget = widget->getChild(path.substr(start, end));
        
        start = end + 1;
    } while(widget && end != std::string::npos);
    
    return widget;
}

}  // namespace GUI
}  // namespace Project
