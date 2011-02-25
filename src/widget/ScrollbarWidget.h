#ifndef PROJECT_WIDGET__SCROLLBAR_WIDGET_H
#define PROJECT_WIDGET__SCROLLBAR_WIDGET_H

#include "AbstractWidget.h"
#include "BoxWidget.h"
#include "ButtonWidget.h"

namespace Project {
namespace Widget {

class ScrollbarLayout;

class ScrollbarWidget : public AbstractWidget {
private:
    friend class ScrollbarLayout;
private:
    bool vertical;
    ButtonWidget *less, *more;
    BoxWidget *bar, *slider;
    double value, size, max;
public:
    ScrollbarWidget(const std::string &name, bool vertical,
        const WidgetRect &bounds);
    
    bool isVertical() const { return vertical; }
    bool isHorizontal() const { return !vertical; }
    
    void setValue(double value);
    void setSize(double size);
    void setMax(double max, bool scaleExisting = false);
    void setEverything(double value, double size, double max);
    
    void render(WidgetVisitor &renderer);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
protected:
    ButtonWidget *getLess() const { return less; }
    ButtonWidget *getMore() const { return more; }
    BoxWidget *getBar() const { return bar; }
    BoxWidget *getSlider() const { return slider; }
private:
    void ensureValid();
    void updateScrolling();
};

}  // namespace Widget
}  // namespace Project

#endif
