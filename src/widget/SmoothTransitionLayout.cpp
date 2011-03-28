#include "SmoothTransitionLayout.h"

#include "misc/Sleeper.h"
#include "log/Logger.h"

namespace Project {
namespace Widget {

SmoothTransitionLayout::SmoothTransitionLayout(
    boost::shared_ptr<Layout> normalLayout, int transitionTime,
    TransitionType type) : normalLayout(normalLayout), type(type),
    inTransition(false), startTransitionAt(0), transitionTime(transitionTime) {
    
}

void SmoothTransitionLayout::update() {
    normalLayout->update();
}

void SmoothTransitionLayout::update(const WidgetRect &newBounds) {
    normalLayout->update(newBounds);
}

void SmoothTransitionLayout::doTransition(int startDelay) {
    WidgetRect profile = profileStack.back();
    if(profileStack.size() > 1) profileStack.pop_back();
    
    doTransition(startDelay, profile);
}

void SmoothTransitionLayout::doTransition(int startDelay,
    const WidgetRect &profile) {
    
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    inTransition = true;
    startTransitionAt = now + startDelay;
    
    originalProfile = normalLayout->getBoundingRect();
    profileStack.push_back(profile);
}

void SmoothTransitionLayout::preRender() const {
    unsigned long now = Misc::Sleeper::getTimeMilliseconds();
    
    if(!inTransition || now <= startTransitionAt) {
        return;
    }
    else if(now <= startTransitionAt + transitionTime) {
        double percent = (now - startTransitionAt)
            / static_cast<double>(transitionTime);
        
        // use a quadratic spline function to smooth the interpolation
        
        double u = 0.0;
        double p = 0.0;
        switch(type) {
        case SMOOTH_START:
            u = percent;
            p = u * u;
            break;
        case SMOOTH_BOTH:
            u = percent * 2;
            
            if(u < 1.0) {
                p = u*u / 2.0;
            }
            else {
                p = 1.0 - (2.0 - u)*(2.0 - u) / 2.0;
            }
            break;
        case SMOOTH_END:
            u = 1.0 - percent;
            p = 1.0 - u * u;
            break;
        }
        
        WidgetRect target = getProfile();
        
        WidgetPoint position = target.getCorner() * p
            + originalProfile.getCorner() * (1.0 - p);
        WidgetPoint dimensions = target.getDimensions() * p
            + originalProfile.getDimensions() * (1.0 - p);
        
        normalLayout->update(WidgetRect(position, dimensions));
    }
    else {
        inTransition = false;
        normalLayout->update(getProfile());
    }
}

WidgetRect SmoothTransitionLayout::getBoundingRect() const {
    return normalLayout->getBoundingRect();
}

}  // namespace Widget
}  // namespace Project
