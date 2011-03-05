#include "CheckWidget.h"

#include "CheckEventProxy.h"

#include "NormalTextLayout.h"
#include "AbsoluteLayout.h"

namespace Project {
namespace Widget {

CheckWidget::CheckWidget(const std::string &name, const std::string &label,
    bool checked, const WidgetRect &bounds) : AbstractWidget(name) {
    
    this->checked = checked;
    
    double shiftedX = bounds.getCorner().getX() + bounds.getHeight() * 1.1;
    double y = bounds.getCorner().getY();
    
    double width = bounds.getWidth() - bounds.getHeight() * 1.1;
    double height = bounds.getHeight();
    
    box = new ButtonWidget("check-box", checked ? "X" : "",
        WidgetRect(bounds.getCorner(), WidgetPoint(height, height)));
    this->label = new TextWidget("check-label", label,
        NormalTextLayout::ALIGN_LEFT,
        WidgetRect(WidgetPoint(shiftedX, y), WidgetPoint(width, height)));
    
    setLayout(new AbsoluteLayout(bounds));
    addEventProxy(new CheckEventProxy(this));
}

void CheckWidget::toggleChecked() {
    checked = !checked;
    
    box->getText()->setText(checked ? "X" : "");
}

void CheckWidget::render(WidgetVisitor &renderer) {
    box->accept(renderer);
    label->accept(renderer);
}

}  // namespace Widget
}  // namespace Project
