#include "ButtonWidget.h"

namespace Project {
namespace Widget {

ButtonWidget::ButtonWidget(const std::string &name, const std::string &data,
    const WidgetRect &bounds) : AbstractWidget(name) {
    
    box = new BoxWidget("button-box", bounds);
    text = new TextWidget("button-text", OpenGL::Color::WHITE, data);
    text->setBoundingRect(bounds);
}

ButtonWidget::~ButtonWidget() {
    delete box;
    delete text;
}

}  // namespace Widget
}  // namespace Project
