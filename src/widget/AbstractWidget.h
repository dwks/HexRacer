#ifndef PROJECT_WIDGET__ABSTRACT_WIDGET_H
#define PROJECT_WIDGET__ABSTRACT_WIDGET_H

#include "WidgetBase.h"

namespace Project {
namespace Widget {

class AbstractWidget : public WidgetBase {
private:
    WidgetRect bounds;
    std::string name;
public:
    AbstractWidget(const std::string &name) : name(name) {}
    
    virtual WidgetRect getBoundingRect() const { return bounds; }
    virtual void setBoundingRect(const WidgetRect &bounds)
        { this->bounds = bounds; }
    
    virtual std::string getName() const { return name; }
protected:
    void setName(const std::string &newName) { name = newName; }
};

}  // namespace Widget
}  // namespace Project

#endif
