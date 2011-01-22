#include "Ray.h"

namespace Project {
namespace Math {

	Ray::Ray(Point _origin, Point _direction) {
		origin = _origin;
		direction = _direction.normalized();
		minBounded = false;
		maxBounded = false;
	}

	Ray::Ray(Point _origin, Point _direction, double min_t) {
		origin = _origin;
		direction = _direction.normalized();
		minBounded = true;
		maxBounded = false;
		minT = min_t;
	}

	Ray::Ray(Point _origin, Point _direction, double min_t, double max_t) {
		origin = _origin;
		direction = _direction.normalized();
		minBounded = true;
		maxBounded = true;
		minT = min_t;
		maxT = max_t;
	}

	bool Ray::insideRange(double t) {
		return ((!minBounded || t >= minT) && (!maxBounded || t <= maxT));
	}

	Point Ray::atT(double t) {
		return (origin+direction*t);
	}

}  // namespace Math
}  // namespace Project
