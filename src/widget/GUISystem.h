#ifndef PROJECT_WIDGET__GUISYSTEM_H
#define PROJECT_WIDGET__GUISYSTEM_H

#include "WidgetBase.h"
#include "render/FontManager.h"

namespace Project {
namespace Widget {

class GUISystem {
private:
    Render::FontManager *fontManager;
    WidgetBase *widgets;
public:
    GUISystem() : widgets(0) {}
    ~GUISystem();
    
    void construct();
    
    void render();
};

}  // namespace Widget
}  // namespace Project

#endif
