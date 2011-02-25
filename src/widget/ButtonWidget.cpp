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
    WidgetRect textBounds = bounds;
    textBounds.getCorner() += textBounds.getDimensions() * 0.15;
    textBounds.getDimensions() *= 0.7;
    text->updateLayout(textBounds);
    
    setLayout(new AbsoluteLayout(bounds));
    
    addEventProxy(new ButtonEventProxy(this));
}

ButtonWidget::~ButtonWidget() {
    delete box;
    delete text;
}

void ButtonWidget::updateLayout(const WidgetRect &newBounds) {
    box->updateLayout(newBounds);
    
    WidgetRect textBounds = newBounds;
    textBounds.getCorner() += textBounds.getDimensions() * 0.15;
    textBounds.getDimensions() *= 0.7;
    text->updateLayout(textBounds);
    
    getLayout()->update(newBounds);
}

}  // namespace Widget
}  // namespace Project
