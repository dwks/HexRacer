#ifndef PROJECT_MATH__RAY_H
#define PROJECT_MATH__RAY_H

#include "Point.h"
#include <float.h>

namespace Project {
namespace Math {

class Ray {
public:
	Ray(Point _origin, Point _direction, bool _bounded = false, double min_t = 0.0, double max_t = FLT_MAX);
	bool insideRange(double t);
	Point atT(double t);

	Point origin;
	Point direction;
	double minT;
	double maxT;
	bool bounded;
};

}  // namespace Math
}  // namespace Project

#endif
