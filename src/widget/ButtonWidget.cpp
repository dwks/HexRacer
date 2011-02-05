#include "ButtonWidget.h"
#include "AbsoluteLayout.h"
#include "ButtonEventProxy.h"

namespace Project {
namespace Widget {

ButtonWidget::ButtonWidget(const std::string &name, const std::string &data,
    const WidgetRect &bounds) : AbstractWidget(name) {
    
    box = new BoxWidget("button-box");
    box->setLayout(new AbsoluteLayout(bounds));
    
    text = new TextWidget("button-text", OpenGL::Color::WHITE, data,
        NormalTextLayout::ALIGN_HCENTRE | NormalTextLayout::ALIGN_VCENTRE);
    text->updateLayout(bounds);
    
    setLayout(new AbsoluteLayout(bounds));
    
    addEventProxy(new ButtonEventProxy(this));
}

ButtonWidget::~ButtonWidget() {
    delete box;
    delete text;
}

}  // namespace Widget
}  // namespace Project
