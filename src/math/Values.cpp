#include <cstdlib>

#include "Values.h"

namespace Project {
namespace Math {

double smartModf(double f, double d) {
	if (f < 0.0f)
		f += std::ceil(std::fabs(f)/d) * d;

	return std::fmod(f, d);
}

int smartModI(int i, int d) {
	if (i < 0)
		i += ((int) std::ceil((double)std::abs(i)/(double)d))*d;

	return i % d;
}

int roundToInt(double f) {
	return static_cast<int>(std::round(f));
}

}  // namespace Math
}  // namespace Project
