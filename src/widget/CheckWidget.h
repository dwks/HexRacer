#ifndef PROJECT_WIDGET__CHECK_WIDGET_H
#define PROJECT_WIDGET__CHECK_WIDGET_H

#include "AbstractWidget.h"

#include "ButtonWidget.h"
#include "TextWidget.h"

namespace Project {
namespace Widget {

class CheckWidget : public AbstractWidget {
private:
    ButtonWidget *box;
    TextWidget *label;
    bool checked;
public:
    CheckWidget(const std::string &name, const std::string &label,
        bool checked, const WidgetRect &bounds);
    
    ButtonWidget *getBox() const { return box; }
    TextWidget *getLabel() const { return label; }
    
    bool isChecked() const { return checked; }
    void toggleChecked();
    
    void render(WidgetVisitor &renderer);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
