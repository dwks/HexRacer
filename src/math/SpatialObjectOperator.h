#ifndef PROJECT_MATH__SPATIAL_OBJECT_OPERATOR_H
#define PROJECT_MATH__SPATIAL_OBJECT_OPERATOR_H

#include "ObjectSpatial.h"

namespace Project {
namespace Math {

class SpatialObjectOperator {
public:
	virtual void operateOnObject(ObjectSpatial* object) = 0;
};

}  // namespace Math
}  // namespace Project

#endif
