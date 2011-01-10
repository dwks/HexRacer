#include "BSPTree3D.h"
using namespace Project;
using namespace Math;

BSPTree3D::BSPTree3D(BoundingBox3D bounding_box, SplitMethod split_method, int split_count, double min_length)
	: BSPTree(split_count)
{
	splitMethod = split_method;
	boundingBox = bounding_box;
	minLength = min_length;
}

BSPTree3D::~BSPTree3D(void)
{
}

bool BSPTree3D::allowSplit() {
	return (
		boundingBox.widthX() >= minLength &&
		boundingBox.widthY() >= minLength &&
		boundingBox.widthZ() >= minLength
		);
}

void BSPTree3D::split() {

	leaf = false;

	Point split_min;
	Point split_max;
	SplitMethod next_split_method = splitMethod;
	Axis split_axis = X_AXIS;
	Axis largest_axis = X_AXIS;
	double largest_axis_width = boundingBox.widthX();

	switch (splitMethod) {

		case FAIR_XSTART:
			next_split_method = FAIR_YSTART;
			split_axis = X_AXIS;
			;break;

		case FAIR_YSTART:
			next_split_method = FAIR_ZSTART;
			split_axis = Y_AXIS;
			;break;

		case FAIR_ZSTART:
			next_split_method = FAIR_XSTART;
			split_axis = Z_AXIS;
			;break;

		case LARGEST_AXIS:

			largest_axis = X_AXIS;
			for (int i = 1; i < 3; i++) {
				double width = boundingBox.widthCoord((Axis) i);
				if (width > largest_axis_width) {
					largest_axis = (Axis) i;
					largest_axis_width = width;
				}
			}
			split_axis = largest_axis;
			;break;
	}

	double mid_val = (boundingBox.minCoord(split_axis) + boundingBox.maxCoord(split_axis))*0.5f;

	switch (split_axis) {

		case X_AXIS:
			split_min = Point(mid_val, boundingBox.minY(), boundingBox.minZ());
			split_max = Point(mid_val, boundingBox.maxY(), boundingBox.maxZ());
			;break;

		case Y_AXIS:
			split_min = Point(boundingBox.minX(), mid_val, boundingBox.minZ());
			split_max = Point(boundingBox.maxX(), mid_val, boundingBox.maxZ());
			;break;

		case Z_AXIS:
			split_min = Point(boundingBox.minX(), boundingBox.minY(), mid_val);
			split_max = Point(boundingBox.maxX(), boundingBox.maxY(), mid_val);
			;break;
	}

	child[0] = new BSPTree3D(
		BoundingBox3D(
			Point(boundingBox.minX(), boundingBox.minY(), boundingBox.minZ()), split_max
		),
		next_split_method, splitCount, minLength);
	child[1] = new BSPTree3D(
		BoundingBox3D(
			Point(boundingBox.maxX(), boundingBox.maxY(), boundingBox.maxZ()), split_min
		),
		next_split_method, splitCount, minLength);
}

BoundingObject& BSPTree3D::getBoundingObject() {
	return boundingBox;
}