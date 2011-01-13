#include "Converter.h"

namespace Project {
namespace Physics {

Math::Point Converter::toPoint(const btVector3 &vector) {
    return Math::Point(
        vector.getX(),
        vector.getY(),
        vector.getZ());
}

btVector3 Converter::toVector(const Math::Point &point) {
    return btVector3(
        point.getX(),
        point.getY(),
        point.getZ());
}

}  // namespace Physics
}  // namespace Project
