#ifndef PROJECT_MATH__CULLABLE_H
#define PROJECT_MATH__CULLABLE_H

#include "SpatialContainer.h"

namespace Project {
namespace Math {

class Cullable {
public:

	virtual void setCullingObject(const BoundingObject* culling_object) = 0;
	virtual void setCullingQueryType(SpatialContainer::QueryType type) = 0;

};

}  // namespace Math
}  // namespace Project

#endif
