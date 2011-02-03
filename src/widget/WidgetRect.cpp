#include <ostream>
#include "WidgetRect.h"

namespace Project {
namespace Widget {

std::ostream &operator << (std::ostream &stream, const WidgetRect &rect) {
    stream << '(' << rect.getCorner() << " by " << rect.getDimensions() << ')';
    return stream;
}

}  // namespace Widget
}  // namespace Project
