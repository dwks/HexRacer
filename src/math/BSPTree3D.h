#pragma once
#include "BSPTree.h"
#include "BoundingBox3D.h"

namespace Project {
namespace Math {

/** A 3D binary partition tree of spatial objects
*/
class BSPTree3D :
	public BSPTree
{
public:

	enum SplitMethod {FAIR_XSTART, FAIR_YSTART, FAIR_ZSTART, LARGEST_AXIS, MIN_OVERLAP};

	/** Class constructor
		@param bounding_box A bounding box that envelopes all objects this tree will contain
		@param split_method The method for splitting the tree:
			FAIR_XSTART/FAIR_YSTART/FAIR_ZSTART: sequential, circular ordering of split axes
			LARGEST_AXIS: split by the largest axis of the bounding box
			MIN_OVERLAP: The split plane that intersects the minimum number of contained objects
				(only works when adding vectors of objects)
	*/
	BSPTree3D(BoundingBox3D bounding_box, SplitMethod split_method = FAIR_XSTART, int split_count = 4, double min_length = 0.0001f);
	~BSPTree3D(void);

	bool allowSplit() const;
	void split(vector<ObjectSpatial*>* objects = NULL);
	void resize(BoundingBox3D bounding_box);
	const BoundingObject& getBoundingObject() const;

private:

	BoundingBox3D boundingBox;
	SplitMethod splitMethod;
	double minLength;

};

}  // namespace Math
}  // namespace Project
