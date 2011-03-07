#include "CentreUponChangeLayout.h"

#include "misc/Sleeper.h"
#include "log/Logger.h"

namespace Project {
namespace Widget {

CentreUponChangeLayout *CentreUponChangeLayout::lastChangedLayout = NULL;

CentreUponChangeLayout::CentreUponChangeLayout(
    boost::shared_ptr<Layout> normalLayout,
    int centreTime, int transitionTime, WidgetRect centreRect)
    : normalLayout(normalLayout), centreTime(centreTime),
    transitionTime(transitionTime), centreRect(centreRect) {
    
}

void CentreUponChangeLayout::update() {
    normalLayout->update();
    
    lastChangedLayout = this;
    lastChanged = Misc::Sleeper::getTimeMilliseconds();
}

void CentreUponChangeLayout::update(const WidgetRect &newBounds) {
    normalLayout->update(newBounds);
    update();
}

WidgetRect CentreUponChangeLayout::getBoundingRect() const {
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    if(lastChangedLayout != this) {
        return normalLayout->getBoundingRect();
    }
    
    if(now <= lastChanged + centreTime) {
        return centreRect;
    }
    else if(now <= lastChanged + centreTime + transitionTime) {
        double percent = (now - (lastChanged + centreTime))
            / static_cast<double>(transitionTime);
        
        // use a quadratic spline function to smooth the interpolation
        double u = percent * 2;
        double p = 0.0;
        if(u < 1.0) {
            p = u*u / 2.0;
        }
        else {
            p = 1.0 - (2.0 - u)*(2.0 - u) / 2.0;
        }
        
        //LOG(WIDGET, "transition is " << percent << " complete");
        
        WidgetRect normal = normalLayout->getBoundingRect();
        
        WidgetPoint position = normal.getCorner() * p
            + centreRect.getCorner() * (1.0 - p);
        WidgetPoint dimensions = normal.getDimensions() * p
            + centreRect.getDimensions() * (1.0 - p);
        
        return WidgetRect(position, dimensions);
    }
    else {
        lastChangedLayout = NULL;
        return normalLayout->getBoundingRect();
    }
}

}  // namespace Widget
}  // namespace Project
