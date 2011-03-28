#ifndef PROJECT_MATH__VALUES_H
#define PROJECT_MATH__VALUES_H

#include <cmath>

#ifdef M_PI
    #define PI M_PI
#elif !defined(PI)
    #define PI 3.14159265358979323846264338327950288419716939937510582097494459
#endif

namespace Project {
namespace Math {

template <typename Type>
Type maximum(const Type &one, const Type &two) {
    if(one < two) {
        return two;
    }
    else {
        return one;
    }
}

template <typename Type>
Type minimum(const Type &one, const Type &two) {
    if(two < one) {
        return two;
    }
    else {
        return one;
    }
}

template <typename Type>
Type sign(const Type &value) {
    return value < 0 ? -1 : 1;
}

template <typename Type>
Type bound(const Type &value, const Type &min, const Type &max) {

    if (value < min)
        return min;

    if (value > max)
        return max;

	return value;
}

inline int intabs(int i) {
    return (i < 0 ? -i : i);
}

int nextPowerOf2(int x);

/** Computes the positive difference between @a i and the greatest multiple of
    @a d which is smaller than @a i
*/
int smartModI(int i, int d);

/** Computes the positive difference between @a f and the greatest multiple of
    @a d which is smaller than @a f
*/
double smartMod(double f, double d);

int roundToInt(double f);

double radiansToDegrees(double radians);
double degreesToRadians(double degrees);


}  // namespace Math
}  // namespace Project

#endif
