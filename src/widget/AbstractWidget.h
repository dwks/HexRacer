#ifndef PROJECT_WIDGET__ABSTRACT_WIDGET_H
#define PROJECT_WIDGET__ABSTRACT_WIDGET_H

#include "WidgetBase.h"

namespace Project {
namespace Widget {

class AbstractWidget : public WidgetBase {
private:
    Layout *layout;
    std::string name;
public:
    AbstractWidget(const std::string &name) : name(name) {}
    virtual ~AbstractWidget();
    
    virtual void updateLayout();
    virtual void updateLayout(const WidgetRect &newBounds);
    virtual WidgetRect getBoundingRect() const;
    virtual Layout *getLayout() const { return layout; }
    virtual void setLayout(Layout *layout) { this->layout = layout; }
    
    virtual std::string getName() const { return name; }
protected:
    void setName(const std::string &newName) { name = newName; }
};

}  // namespace Widget
}  // namespace Project

#endif
