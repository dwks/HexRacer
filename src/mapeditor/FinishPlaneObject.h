#pragma once

#include "MapObject.h"
#include "map/HRMap.h"
#include "math/SimpleTransform.h"
using namespace Project;
using namespace Math;
using namespace Map;

class FinishPlaneObject
	: public MapObject
{
private:
	HRMap* map;
	SimpleTransform transform;

	void updatePlaneNormal();
public:
	FinishPlaneObject(HRMap* _map);

	Point getPosition() const { return map->getFinishPlane().centroid(); }
	void setPosition(const Point& new_position) { map->getFinishPlane().moveCentroid(new_position); }
	void translate(const Point& translation) { map->getFinishPlane().translate(translation); }
	BoundingBox3D getBoundingBox() const;

	double getRotation(RotationAxis axis) { return transform.getRotationRadians(axis); }
	void setRotation(double radians, RotationAxis axis);

	//Matrix getTransformMatrix() const { return transform.getMatrix(); }

	MapObject::ObjectType getType() const { return MapObject::FINISH_PLANE; }
	bool hasRotation() const { return true; }
};
