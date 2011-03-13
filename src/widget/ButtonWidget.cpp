#include "ButtonWidget.h"
#include "AbsoluteLayout.h"
#include "ButtonEventProxy.h"
#include "ButtonWidgetLayout.h"

namespace Project {
namespace Widget {

ButtonWidget::ButtonWidget(const std::string &name, const std::string &data,
    const WidgetRect &bounds) : AbstractWidget(name) {
    
    box = new BoxWidget("button-box");
    box->setLayout(new AbsoluteLayout(bounds));
    
    text = new TextWidget("button-text", OpenGL::Color::WHITE, data,
        NormalTextLayout::ALIGN_HCENTRE | NormalTextLayout::ALIGN_VCENTRE);
    
    setLayout(new ButtonWidgetLayout(this, bounds));
    
    addEventProxy(new ButtonEventProxy(this));
}

ButtonWidget::~ButtonWidget() {
    delete box;
    delete text;
}

}  // namespace Widget
}  // namespace Project
