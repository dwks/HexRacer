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

/** Computes the positive difference between @a f and the greatest multiple of
    @a d which is smaller than @a f
*/

/*
template <typename Type>
Type smartMod(Type f, Type d) {
    if (i < 0)
        i += ((int) std::ceil((double)std::abs(i)/(double)d))*d;
    
    return i % d;
}

template <>
float smartMod<float>(float f, float d) {
    if (f < 0.0f)
        f += std::ceil(std::fabs(f)/d) * d;
    
    return std::fmod(f, d);
}

template <>*/
double smartMod/*<double>*/(double f, double d);

int roundToInt(double f);

}  // namespace Math
}  // namespace Project

#endif
