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

bool BSPTree3D::allowSplit() const {
	return (
		boundingBox.widthX() >= minLength ||
		boundingBox.widthY() >= minLength ||
		boundingBox.widthZ() >= minLength
		);
}

void BSPTree3D::split(vector<ObjectSpatial*>* objects) {

	leaf = false;

	Point split_min;
	Point split_max;
	SplitMethod next_split_method = splitMethod;
	Axis split_axis = X_AXIS;
	Axis largest_axis = X_AXIS;
	Axis min_overlap_axis = X_AXIS;
	int min_overlap_num = 0;
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
			for (unsigned int i = 1; i < 3; i++) {
				double width = boundingBox.widthCoord((Axis) i);
				if (width > largest_axis_width) {
					largest_axis = (Axis) i;
					largest_axis_width = width;
				}
			}
			split_axis = largest_axis;
			;break;

		case MIN_OVERLAP:
			if (objects == NULL) {
				next_split_method = FAIR_XSTART;
				split_axis = X_AXIS;
			}
			else {
				for (unsigned int i = 0; i < 3; i++) {
					Axis axis = (Axis) i;
					int overlap_num = 0;
					double test_mid = (boundingBox.minCoord(axis) + boundingBox.maxCoord(axis))*0.5f;
					for (unsigned int j = 0; j < objects->size(); j++) {
						if (objects->at(j)->minCoord(axis) < test_mid && objects->at(j)->maxCoord(axis) > test_mid) {
							overlap_num++;
						}
					}
					if (i == 0 || overlap_num < min_overlap_num) {
						min_overlap_num = overlap_num;
						min_overlap_axis = axis;
					}
				}
				next_split_method = MIN_OVERLAP;
				split_axis = min_overlap_axis;
				
			}
			;break;

	}

	//Don't split on an axis that is below the min length
	while (boundingBox.widthCoord(split_axis) < minLength) {
		split_axis = (Axis) ((((int) split_axis)+1) % 3);
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

const BoundingObject& BSPTree3D::getBoundingObject() const {
	return boundingBox;
}

void BSPTree3D::resize(BoundingBox3D bounding_box) {
	vector<ObjectSpatial*> all_objects = all();
	clear();
	boundingBox = bounding_box;
	add(all_objects);
}