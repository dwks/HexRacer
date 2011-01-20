#ifndef PROJECT_PHYSICS__CONVERTER_H
#define PROJECT_PHYSICS__CONVERTER_H

#include "math/Point.h"
#include "math/Matrix.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"

namespace Project {
namespace Physics {

class Converter {
public:
    static Math::Point toPoint(const btVector3 &vector);
    static btVector3 toVector(const Math::Point &point);
    static Math::Matrix toMatrix(const btTransform &transform);
    static btTransform toTransform(const Math::Matrix &matrix);
};

}  // namespace Physics
}  // namespace Project

#endif
