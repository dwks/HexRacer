#include "GUISystem.h"
#include "WidgetRenderer.h"

#include "BoxWidget.h"

namespace Project {
namespace Widget {

GUISystem::~GUISystem() {
    delete widgets;
}

void GUISystem::construct() {
    widgets = new BoxWidget("test box", WidgetRect(0.3, 0.4, 0.4, 0.1));
}

void GUISystem::render() {
    WidgetRenderer renderer;
    
    renderer.begin();
    widgets->accept(renderer);
    renderer.end();
}

}  // namespace Widget
}  // namespace Project
