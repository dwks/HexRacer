#include <ostream>
#include "WidgetRect.h"

namespace Project {
namespace Widget {

bool WidgetRect::pointInside(const WidgetPoint &point) const {
    return point.getX() >= corner.getX()
        && point.getY() >= corner.getY()
        && point.getX() <= corner.getX() + dimensions.getX()
        && point.getY() <= corner.getY() + dimensions.getY();
}

std::ostream &operator << (std::ostream &stream, const WidgetRect &rect) {
    stream << '(' << rect.getCorner() << " by " << rect.getDimensions() << ')';
    return stream;
}

}  // namespace Widget
}  // namespace Project
