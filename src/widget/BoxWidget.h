#ifndef PROJECT_WIDGET__BOX_WIDGET_H
#define PROJECT_WIDGET__BOX_WIDGET_H

#include "AbstractWidget.h"

namespace Project {
namespace Widget {

class BoxWidget : public AbstractWidget {
public:
    BoxWidget(const std::string &name) : AbstractWidget(name) {}
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
};

}  // namespace Widget
}  // namespace Project

#endif
