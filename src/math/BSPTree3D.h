#pragma once
#include "BSPTree.h"
#include "BoundingBox3D.h"

namespace Project {
namespace Math {

class BSPTree3D :
	public BSPTree
{
public:

	enum SplitMethod {FAIR_XSTART, FAIR_YSTART, FAIR_ZSTART, LARGEST_AXIS, MIN_OVERLAP};

	BSPTree3D(BoundingBox3D bounding_box, SplitMethod split_method = FAIR_XSTART, int split_count = 4, double min_length = 0.0001f);
	~BSPTree3D(void);

	bool allowSplit() const;
	void split(vector<ObjectSpatial*>* objects = NULL);
	const BoundingObject& getBoundingObject() const;

private:

	BoundingBox3D boundingBox;
	SplitMethod splitMethod;
	double minLength;

};

}  // namespace Math
}  // namespace Project
