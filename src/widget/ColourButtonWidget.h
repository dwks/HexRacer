#ifndef PROJECT_WIDGET__COLOUR_BUTTON_WIDGET_H
#define PROJECT_WIDGET__COLOUR_BUTTON_WIDGET_H

#include "ButtonWidget.h"

namespace Project {
namespace Widget {

class WidgetRenderer;

class ColourButtonWidget : public ButtonWidget {
private:
    OpenGL::Color colour;
public:
    ColourButtonWidget(const std::string &name,
        OpenGL::Color colour,
        const std::string &data,
        const WidgetRect &bounds);
    
    void render(WidgetRenderer &renderer);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
