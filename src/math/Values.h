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

}  // namespace Math
}  // namespace Project

#endif
