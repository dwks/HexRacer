#ifndef PROJECT_WIDGET__SMOOTH_TRANSITION_LAYOUT_H
#define PROJECT_WIDGET__SMOOTH_TRANSITION_LAYOUT_H

#include "Layout.h"

#include <vector>
#include "boost/shared_ptr.hpp"

namespace Project {
namespace Widget {

class SmoothTransitionLayout : public Layout {
public:
    enum TransitionType {
        SMOOTH_START,
        SMOOTH_BOTH,
        SMOOTH_END
    };
private:
    boost::shared_ptr<Layout> normalLayout;
    TransitionType type;
    mutable bool inTransition;
    unsigned long startTransitionAt;
    int transitionTime;
    std::vector<WidgetRect> profileStack;
    WidgetRect originalProfile;
public:
    SmoothTransitionLayout(boost::shared_ptr<Layout> normalLayout,
        int transitionTime, TransitionType type = SMOOTH_BOTH);
    
    virtual void update();
    virtual void update(const WidgetRect &newBounds);
    
    boost::shared_ptr<Layout> getNormalLayout() { return normalLayout; }
    
    /** Pops the profile to use from the profile stack.
    */
    virtual void doTransition(int startDelay);
    
    /** Pushes @a profile onto the profile stack.
    */
    virtual void doTransition(int startDelay, const WidgetRect &profile);
    
    virtual void preRender() const;
    
    virtual WidgetRect getBoundingRect() const;
private:
    const WidgetRect &getProfile() const { return profileStack.back(); }
};

}  // namespace Widget
}  // namespace Project

#endif
