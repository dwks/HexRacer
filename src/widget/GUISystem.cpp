#include "GUISystem.h"
#include "WidgetRenderer.h"

#include "ButtonWidget.h"

namespace Project {
namespace Widget {

GUISystem::~GUISystem() {
    delete widgets;
    delete fontManager;
}

void GUISystem::construct() {
    fontManager = new Render::FontManager();
    
    widgets = new CompositeWidget("gui");
    
    widgets->addChild(new ButtonWidget("help",
        "?", WidgetRect(0.0, 0.0, 0.1, 0.1)));
    
    widgets->addChild(new ButtonWidget("resume",
        "Resume", WidgetRect(0.3, 0.4 - 0.15, 0.4, 0.1)));
    
    widgets->addChild(new ButtonWidget("settings",
        "Settings", WidgetRect(0.3, 0.4, 0.4, 0.1)));
    
    widgets->addChild(new ButtonWidget("quit",
        "Quit", WidgetRect(0.3, 0.4 + 0.15, 0.4, 0.1)));
}

void GUISystem::render() {
    WidgetRenderer renderer(screenSize);
    
    renderer.begin();
    widgets->accept(renderer);
    renderer.end();
}

void GUISystem::setScreenSize(int width, int height) {
    screenSize = WidgetPoint(width, height);
}

}  // namespace Widget
}  // namespace Project
