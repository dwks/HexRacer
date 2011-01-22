#ifndef PROJECT_MATH__RAY_H
#define PROJECT_MATH__RAY_H

#include "Point.h"
#include <float.h>

namespace Project {
namespace Math {

class Ray {
public:
	Ray(Point _origin, Point _direction);
	Ray(Point _origin, Point _direction, double min_t);
	Ray(Point _origin, Point _direction, double min_t, double max_t);
	bool insideRange(double t);
	Point atT(double t);

	Point origin;
	Point direction;
	double minT;
	double maxT;
	bool minBounded;
	bool maxBounded;
};

}  // namespace Math
}  // namespace Project

#endif
