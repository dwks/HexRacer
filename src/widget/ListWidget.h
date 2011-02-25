#ifndef PROJECT_WIDGET__LIST_WIDGET_H
#define PROJECT_WIDGET__LIST_WIDGET_H

#include "CompositeWidget.h"
#include "ScrollbarWidget.h"

namespace Project {
namespace Widget {

/** A listbox widget which displays a view into a (possibly quite long)
    scrolling list of other widgets.
*/
class ListWidget : public CompositeWidget {
private:
    int lines;
    ScrollbarWidget *verticalBar, *horizontalBar;
    WidgetRect viewArea;
public:
    ListWidget(const std::string &name, int lines,
        bool vertical, bool horizontal, const WidgetRect &bounds);
    
    // overridden to automatically position the widget correctly
    virtual void addChild(WidgetBase *widget);
};

}  // namespace Widget
}  // namespace Project

#endif
