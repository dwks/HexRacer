#include "GUISystem.h"
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

}  // namespace GUI
}  // namespace Project
