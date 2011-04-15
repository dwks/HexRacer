#ifndef PROJECT_WIDGET__LIST_WIDGET_H
#define PROJECT_WIDGET__LIST_WIDGET_H

#include "CompositeWidget.h"
#include "BoxWidget.h"
#include "ScrollbarWidget.h"

namespace Project {
namespace Widget {

class ListEventProxy;
class ListItemEventProxy;
class ListScrollbarEventProxy;
class ListWidgetBoxProxy;

/** A listbox widget which displays a view into a (possibly quite long)
    scrolling list of other widgets.
*/
class ListWidget : public CompositeWidget {
private:
    friend class ListEventProxy;
    friend class ListItemEventProxy;
    friend class ListScrollbarEventProxy;
    friend class ListWidgetBoxProxy;
private:
    BoxWidget *box;
    ScrollbarWidget *verticalBar, *horizontalBar;
    WidgetRect viewArea;
    double totalHeight, maxWidth;
    WidgetBase *lastSelected;
public:
    ListWidget(const std::string &name, bool vertical, bool horizontal,
        const WidgetRect &bounds);
    
    using CompositeWidget::addChild;
    /** Overridden to automatically position the widget correctly; any child
        widget added to a ListWidget need only have its dimensions set, the
        position will be set appropriately.
    */
    virtual void addChild(boost::shared_ptr<WidgetBase> widget);
    
    virtual void removeAllChildren();
    
    void render(WidgetVisitor &renderer);
    
    void setLastSelected(WidgetBase *widget);
    void setLastSelected(const std::string &text);
    
    virtual void accept(WidgetVisitor &visitor) { visitor.visit(this); }
protected:
    void repositionChildren();
    
    BoxWidget *getBox() const { return box; }
    ScrollbarWidget *getVerticalBar() const { return verticalBar; }
    ScrollbarWidget *getHorizontalBar() const { return horizontalBar; }
    WidgetRect getViewArea() const { return viewArea; }
};

}  // namespace Widget
}  // namespace Project

#endif
