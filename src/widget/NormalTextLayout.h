#ifndef PROJECT_WIDGET__NORMAL_TEXT_LAYOUT_H
#define PROJECT_WIDGET__NORMAL_TEXT_LAYOUT_H

#include "Layout.h"

namespace Project {
namespace Widget {

/** Restricts its contents to a hard bounding box while preserving aspect
    ratio. Leftover space can be placed to the right or left, top or bottom
    or various combinations thereof.
*/
class NormalTextLayout : public Layout {
public:
    enum align_t {
        ALIGN_LEFT      = 1 << 0,
        ALIGN_HCENTRE   = 1 << 1,
        ALIGN_RIGHT     = 1 << 2,
        ALIGN_TOP       = 1 << 3,
        ALIGN_VCENTRE   = 1 << 4,
        ALIGN_BOTTOM    = 1 << 5
    };
private:
    WidgetRect givenBounds;
    WidgetRect bounds;
    unsigned align;
    double aspectRatio;
public:
    NormalTextLayout(unsigned align, double aspectRatio)
        : align(align), aspectRatio(aspectRatio) {}
    
    void setAspectRatio(double ratio) { aspectRatio = ratio; }
    double getAspectRatio() const { return aspectRatio; }
    
    virtual void update();
    virtual void update(const WidgetRect &newBounds);
    
    virtual WidgetRect getBoundingRect() const { return bounds; }
};

}  // namespace Widget
}  // namespace Project

#endif
