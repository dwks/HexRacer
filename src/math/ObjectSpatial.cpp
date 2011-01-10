#include "ObjectSpatial.h"
using namespace Project;
using namespace Math;

double ObjectSpatial::minCoord(Axis axis) {
	switch (axis) {
		case X_AXIS:
			return minX();
		case Y_AXIS:
			return minY();
		case Z_AXIS:
			return minZ();
	}
	return 0.0f;
}

double ObjectSpatial::maxCoord(Axis axis) {
	switch (axis) {
		case X_AXIS:
			return maxX();
		case Y_AXIS:
			return maxY();
		case Z_AXIS:
			return maxZ();
	}
	return 0.0f;
}

double ObjectSpatial::widthCoord(Axis axis) {
	switch (axis) {
		case X_AXIS:
			return widthX();
		case Y_AXIS:
			return widthY();
		case Z_AXIS:
			return widthZ();
	}
	return 0.0f;
}