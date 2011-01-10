#include "Values.h"

namespace Project {
namespace Math {


double smartModf(double f, double d) {
	if (f < 0.0f)
		f += ceil(fabs(f)/d) * d;

	return fmod(f, d);
}

long smartModI(long i, long d) {
	if (i < 0)
		i += ((long) ceil((double)abs(i)/(double)d))*d;

	return i % d;
}

double round(double f) {
	if (smartModf(f , 1.0f) >= 0.5f)
		return ceil(f);
	else
		return floor(f);
}

long roundToInt(double f) {
	return (long) round(f);
}

}  // namespace Math
}  // namespace Project
