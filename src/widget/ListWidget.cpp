#include "ListWidget.h"

#include "AbsoluteLayout.h"
#include "ListEventProxy.h"
#include "ListScrollbarEventProxy.h"
#include "ListItemEventProxy.h"
#include "ListWidgetBoxProxy.h"

#include "math/Values.h"

#include "log/Logger.h"

#include "opengl/OpenGL.h"
#include "widget/WidgetRenderer.h"
#include "sdl/Projector.h"

namespace Project {
namespace Widget {

ListWidget::ListWidget(const std::string &name, bool vertical, bool horizontal,
    const WidgetRect &bounds) : CompositeWidget(name) {
    
    totalHeight = 0.0;
    maxWidth = 0.0;
    
    verticalBar = 0;
    horizontalBar = 0;
    
    lastSelected = NULL;
    
    WidgetRect boxBounds = bounds;
    
    boost::shared_ptr<EventProxy> scrollbarEventProxy(
        new ListScrollbarEventProxy(this));
    if(vertical) {
        WidgetRect verticalBounds = bounds;
        verticalBounds.getDimensions().setX(0.06);
        verticalBounds.getCorner().addX(
            bounds.getWidth() - verticalBounds.getWidth());
        
        boxBounds.getDimensions().addX(-verticalBounds.getWidth());
        
        verticalBar = new ScrollbarWidget("list-vertical-scrollbar", true,
            verticalBounds);
        verticalBar->getSlider()
            ->addEventProxy(scrollbarEventProxy);
        verticalBar->getLess()
            ->addEventProxy(scrollbarEventProxy);
        verticalBar->getMore()
            ->addEventProxy(scrollbarEventProxy);
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
            ->addEventProxy(scrollbarEventProxy);
        horizontalBar->getLess()
            ->addEventProxy(scrollbarEventProxy);
        horizontalBar->getMore()
            ->addEventProxy(scrollbarEventProxy);
    }
    
    box = new BoxWidget("list-box", "corners/in/normal");
    box->setLayout(new AbsoluteLayout(boxBounds));
    box->addEventProxy(new ListWidgetBoxProxy(this));
    
    viewArea = boxBounds;
    //double addW = viewArea.getWidth() * 0.05;
    //double addH = viewArea.getHeight() * 0.15;
    //double size = Math::maximum(viewArea.getWidth(), viewArea.getHeight());
    double addW = 0.03;
    double addH = 0.04;
    
    viewArea.getCorner().addX(addW);
    viewArea.getCorner().addY(addH);
    viewArea.getDimensions().addX(-addW * 2);
    viewArea.getDimensions().addY(-addH * 2);
    
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

void ListWidget::addChild(boost::shared_ptr<WidgetBase> widget) {
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
    
    // next set the new child's position
    WidgetRect rect = widget->getBoundingRect();
    double y = viewArea.getCorner().getY() + totalHeight;
    if(verticalBar) y -= verticalBar->getValue();
    rect.getCorner().setX(viewArea.getCorner().getX());
    rect.getCorner().setY(y);
    widget->updateLayout(rect);
    
    // finally make the vertical scrollbar taller as necessary
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
    
    // add an event proxy which will scroll this child correctly
    widget->addEventProxy(new ListItemEventProxy(this,
        dynamic_cast<TextWidget *>(widget.get())));
    
    CompositeWidget::addChild(widget);
}

void ListWidget::removeAllChildren() {
    CompositeWidget::removeAllChildren();
    
    totalHeight = 0.0;
    if(verticalBar) {
        verticalBar->setEverything(
            0.0, viewArea.getHeight(), viewArea.getHeight());
    }
    if(horizontalBar) {
        horizontalBar->setEverything(
            0.0, viewArea.getWidth(), viewArea.getWidth());
    }
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
            
            if(childY >= startY && childY + childHeight <= endY + 1e-8) {
                if(child == lastSelected) {
                    WidgetPoint corner = child->getBoundingRect().getCorner();
                    WidgetPoint dim = child->getBoundingRect().getDimensions();
                    
                    dim.setX(viewArea.getWidth());
                    
                    WidgetPoint ul = corner;
                    WidgetPoint ll = corner;
                    WidgetPoint ur = corner;
                    WidgetPoint lr = corner + dim;
                    
                    ll.addY(dim.getY());
                    ur.addX(dim.getX());
                    
                    glColor3f(0.0f, 0.4f, 0.5f);
                    
                    glBegin(GL_QUADS);
                    WidgetRenderer::glVertex(ul);
                    WidgetRenderer::glVertex(ur);
                    WidgetRenderer::glVertex(lr);
                    WidgetRenderer::glVertex(ll);
                    glEnd();
                    
                    glColor3f(1.0f, 1.0f, 1.0f);
                }
                
                child->accept(renderer);
            }
        }
    }
    else {
        LOG2(WIDGET, WARNING, "Displaying ListWidget with no vertical"
            " scrollbar is not implemented");
    }
}

void ListWidget::setLastSelected(WidgetBase *widget) {
    this->lastSelected = widget;
}

void ListWidget::setLastSelected(const std::string &text) {
    IteratorType it = getIterator();
    while(it.hasNext()) {
        TextWidget *widget = dynamic_cast<TextWidget *>(it.next());
        if(widget) {
            if(widget->getData() == text) {
                lastSelected = widget;
                return;
            }
        }
    }
}

}  // namespace Widget
}  // namespace Project
