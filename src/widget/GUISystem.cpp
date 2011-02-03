#include "GUISystem.h"
#include "WidgetRenderer.h"

#include "BoxWidget.h"
#include "TextWidget.h"

namespace Project {
namespace Widget {

GUISystem::~GUISystem() {
    delete widgets;
    delete fontManager;
}

void GUISystem::construct() {
    fontManager = new Render::FontManager();
    
    widgets = new TextWidget("test text", OpenGL::Color::BLUE,
        "Greetings.");
    widgets->setBoundingRect(WidgetRect(0.3, 0.4, 0.4, 0.1));
}

void GUISystem::render() {
    WidgetRenderer renderer;
    
    renderer.begin();
    widgets->accept(renderer);
    renderer.end();
}

}  // namespace Widget
}  // namespace Project
