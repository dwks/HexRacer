#include <cstdlib>
#include "Values.h"

namespace Project {
namespace Math {

int roundToInt(double f) {
	return static_cast<int>(f < 0 ? (f - 0.5) : (f + 0.5));
}

int smartModI(int i, int d) {
    if (i < 0)
        i += ((int) std::ceil((double)std::abs(i)/(double)d))*d;
    
    return i % d;
}

double smartMod(double f, double d) {
    if (f < 0.0f)
        f += std::ceil(std::fabs(f)/d) * d;
    
    return std::fmod(f, d);
}

double radiansToDegrees(double radians) {
	return radians/PI*180.0;
}
double degreesToRadians(double degrees) {
	return degrees/180.0*PI;
}

int nextPowerOf2(int x) {
    return 1 << int(std::ceil((double) std::log((double) x) / std::log((double) 2.0)));
}

}  // namespace Math
}  // namespace Project
