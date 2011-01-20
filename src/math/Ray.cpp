#include "Ray.h"

namespace Project {
namespace Math {

	Ray::Ray(Point _origin, Point _direction, bool _bounded, double min_t, double max_t) {
		origin = _origin;
		direction = _direction.normalized();
		bounded = _bounded;
		minT = min_t;
		maxT = max_t;
	}

	bool Ray::insideRange(double t) {
		return (!bounded || (t >= minT && t <= maxT));
	}

	Point Ray::atT(double t) {
		return (origin+direction*t);
	}

}  // namespace Math
}  // namespace Project
