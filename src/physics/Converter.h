#ifndef PROJECT_PHYSICS__CONVERTER_H
#define PROJECT_PHYSICS__CONVERTER_H

#include "math/Point.h"
#include "LinearMath/btVector3.h"

namespace Project {
namespace Physics {

class Converter {
public:
    static Math::Point toPoint(const btVector3 &vector);
    static btVector3 toVector(const Math::Point &point);
};

}  // namespace Physics
}  // namespace Project

#endif
