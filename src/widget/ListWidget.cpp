#include "ListWidget.h"

#include "AbsoluteLayout.h"
#include "ListEventProxy.h"
#include "ListScrollbarEventProxy.h"

#include "math/Values.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

ListWidget::ListWidget(const std::string &name, bool vertical, bool horizontal,
    const WidgetRect &bounds) : CompositeWidget(name) {
    
    totalHeight = 0.0;
    maxWidth = 0.0;
    
    verticalBar = 0;
    horizontalBar = 0;
    
    WidgetRect boxBounds = bounds;
    
    if(vertical) {
        WidgetRect verticalBounds = bounds;
        verticalBounds.getDimensions().setX(0.06);
        verticalBounds.getCorner().addX(
            bounds.getWidth() - verticalBounds.getWidth());
        
        boxBounds.getDimensions().addX(-verticalBounds.getWidth());
        
        verticalBar = new ScrollbarWidget("list-vertical-scrollbar", true,
            verticalBounds);
        verticalBar->getSlider()
            ->addEventProxy(new ListScrollbarEventProxy(this));
    }
    if(horizontal) {
        WidgetRect horizontalBounds = bounds;
        horizontalBounds.getDimensions().setY(0.06);
        horizontalBounds.getCorner().addY(
            bounds.getHeight() - horizontalBounds.getHeight());
        
        boxBounds.getDimensions().addY(-horizontalBounds.getHeight());
        
        horizontalBar = new ScrollbarWidget("list-horizontal-scrollbar", false,
            horizontalBounds);
        horizontalBar->getSlider()
            ->addEventProxy(new ListScrollbarEventProxy(this));
    }
    
    box = new BoxWidget("list-box", "corners/in/normal");
    box->setLayout(new AbsoluteLayout(boxBounds));
    
    viewArea = boxBounds;
    viewArea.getCorner().addX(viewArea.getWidth() * 0.1);
    viewArea.getCorner().addY(viewArea.getHeight() * 0.15);
    viewArea.getDimensions().addX(-viewArea.getWidth() * 0.1 * 2);
    viewArea.getDimensions().addY(-viewArea.getHeight() * 0.15 * 2);
    
    if(verticalBar) {
        verticalBar->setEverything(
            0.0, viewArea.getHeight(), viewArea.getHeight());
    }
    if(horizontalBar) {
        horizontalBar->setEverything(
            0.0, viewArea.getWidth(), viewArea.getWidth());
    }
    
    setLayout(new AbsoluteLayout(bounds));
    addEventProxy(new ListEventProxy(this));
}

void ListWidget::addChild(WidgetBase *widget) {
    // first check if the list widget needs to be made wider
    // (if there is no horizontal scrollbar, clip child to width)
    if(horizontalBar) {
        maxWidth = Math::maximum(maxWidth, widget->getBoundingRect().getWidth());
    }
    else {
        if(widget->getBoundingRect().getWidth() > viewArea.getWidth()) {
            widget->getBoundingRect().getDimensions()
                .setX(viewArea.getWidth());
        }
    }
    
    WidgetRect rect = widget->getBoundingRect();
    double y = viewArea.getCorner().getY() + totalHeight;
    if(verticalBar) y -= verticalBar->getValue();
    rect.getCorner().setX(viewArea.getCorner().getX());
    rect.getCorner().setY(y);
    widget->updateLayout(rect);
    
    LOG(GUI, "child " << widget->getName() << " set at " << rect);
    
    totalHeight += widget->getBoundingRect().getHeight();
    
    if(totalHeight > viewArea.getHeight()) {
        if(verticalBar) {
            verticalBar->setMax(totalHeight, false);
        }
        else {
            LOG2(WIDGET, WARNING, "ListWidget is overflowing vertically"
                " but has no vertical scrollbar");
        }
    }
    
    CompositeWidget::addChild(widget);
}

void ListWidget::repositionChildren() {
    double xPos = viewArea.getCorner().getX();
    double yPos = viewArea.getCorner().getY();
    if(horizontalBar) xPos -= horizontalBar->getValue();
    if(verticalBar) yPos -= verticalBar->getValue();
    
    //LOG(WIDGET, "y offset is " << yPos);
    
    IteratorType it = getIterator();
    while(it.hasNext()) {
        WidgetBase *child = it.next();
        
        WidgetRect rect = child->getBoundingRect();
        rect.setCorner(WidgetPoint(xPos, yPos));
        child->updateLayout(rect);
        
        yPos += rect.getHeight();
    }
}

void ListWidget::render(WidgetVisitor &renderer) {
    box->accept(renderer);
    
    if(verticalBar) verticalBar->accept(renderer);
    if(horizontalBar) horizontalBar->accept(renderer);
    
    if(verticalBar) {
        double startY = viewArea.getCorner().getY();
        double endY = startY + verticalBar->getSize();
        
        IteratorType it = getIterator();
        while(it.hasNext()) {
            WidgetBase *child = it.next();
            
            double childY = child->getBoundingRect().getCorner().getY();
            double childHeight = child->getBoundingRect().getHeight();
            
            /*LOG(GUI, "considering painting child " << child->getName() << " at "
                << child->getBoundingRect() << " given " << startY << "," << endY);*/
            
            if(childY >= startY && childY + childHeight <= endY) {
                child->accept(renderer);
            }
        }
    }
    else {
        LOG2(WIDGET, WARNING, "Displaying ListWidget with no vertical"
            " scrollbar is not implemented");
    }
}

}  // namespace Widget
}  // namespace Project
