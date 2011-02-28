#include "NormalTextLayout.h"

#include "log/Logger.h"

namespace Project {
namespace Widget {

void NormalTextLayout::update() {
    double newRatio = givenBounds.getHeight() / givenBounds.getWidth();
    bounds = givenBounds;
    
    if(newRatio > aspectRatio) {
        // the X size is already precisely right, shrink Y to meet ratio
        double oldY = givenBounds.getHeight();
        double newY = aspectRatio * givenBounds.getWidth();
        
        if(align & ALIGN_TOP) {
            bounds.getDimensions().setY(newY);
        }
        else if(align & ALIGN_BOTTOM) {
            bounds.getCorner().addY(oldY - newY);
            bounds.getDimensions().setY(newY);
        }
        else {  // default: align to vcentre
            bounds.getCorner().addY((oldY - newY) / 2.0);
            bounds.getDimensions().setY(newY);
        }
    }
    else {
        // the Y size is already precisely right, shrink X to meet ratio
        double oldX = givenBounds.getWidth();
        double newX = givenBounds.getHeight() / aspectRatio;
        
        if(align & ALIGN_LEFT) {
            bounds.getDimensions().setX(newX);
        }
        else if(align & ALIGN_RIGHT) {
            bounds.getCorner().addX(oldX - newX);
            bounds.getDimensions().setX(newX);
        }
        else {  // default: align to hcentre
            bounds.getCorner().addX((oldX - newX) / 2.0);
            bounds.getDimensions().setX(newX);
        }
    }
    
    //LOG(WIDGET, "Text layout gives " << bounds << " in " << hardBounds);
}

void NormalTextLayout::update(const WidgetRect &newBounds) {
    this->givenBounds = newBounds;
    this->bounds = newBounds;
    
    update();
}

}  // namespace Widget
}  // namespace Project
