#ifndef PROJECT_WIDGET__BUTTON_WIDGET_H
#define PROJECT_WIDGET__BUTTON_WIDGET_H

#include "AbstractWidget.h"
#include "BoxWidget.h"
#include "TextWidget.h"

namespace Project {
namespace Widget {

class ButtonWidget : public AbstractWidget {
private:
    BoxWidget *box;
    TextWidget *text;
public:
    ButtonWidget(const std::string &name, const std::string &data,
        const WidgetRect &bounds);
    virtual ~ButtonWidget();
    
    BoxWidget *getBox() { return box; }
    TextWidget *getText() { return text; }
    
    using AbstractWidget::updateLayout;
    virtual void updateLayout(const WidgetRect &newBounds);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
