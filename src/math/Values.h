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

/** Computes the positive difference between @f and the greatest multiple of @d
	which is smaller than @f
*/
double smartModf(double f, double d);

/** Computes the positive difference between @i and the greatest multiple of @d
	which is smaller than @i
*/
long smartModI(long i, long d);
double round(double f);
long roundToInt(double f);

}  // namespace Math
}  // namespace Project

#endif
