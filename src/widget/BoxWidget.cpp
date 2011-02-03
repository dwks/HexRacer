#include "BoxWidget.h"

namespace Project {
namespace Widget {

BoxWidget::BoxWidget(const std::string &name, const WidgetRect &bounds)
    : AbstractWidget(name) {
    
    setBoundingRect(bounds);
}

}  // namespace Widget
}  // namespace Project
