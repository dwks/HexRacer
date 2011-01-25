#ifndef PROJECT_MATH__RAY_INTERSECTION_H
#define PROJECT_MATH__RAY_INTERSECTION_H

namespace Project {
namespace Math {

class RayIntersection {
public:

	bool intersects;
	double t;

	RayIntersection() {
		intersects = false;
	} 

	RayIntersection(double _t) {
		intersects = true;
		t = _t;
	}

	bool operator < (const RayIntersection i) const {
		return (t < i.t);
	}
	
};

}  // namespace Math
}  // namespace Project

#endif
