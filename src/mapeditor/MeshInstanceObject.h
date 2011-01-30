#pragma once

#include "MapObject.h"
#include "render/TransformedMesh.h"
#include "math/SimpleTransform.h"
using namespace Project;
using namespace Render;
using namespace Math;

class MeshInstanceObject
	: public MapObject
{
private:

	TransformedMesh* transformedMesh;
	SimpleTransform transformation;
	void update();

public:

	MeshInstanceObject(TransformedMesh* transformed_mesh);

	TransformedMesh* getTransformedMesh() { return transformedMesh; }

	Point getPosition() const { return transformation.getTranslation(); }
	void setPosition(const Point& new_position);
	void translate(const Point& translation);
	BoundingBox3D getBoundingBox() const;

	bool hasRotation() const { return true; }
	double getRotation(RotationAxis axis) { return transformation.getRotationRadians(axis); }
	void setRotation(double radians, RotationAxis axis);

	virtual bool hasScale() const { return true; }
	virtual double getScale() const { return transformation.getScale(); }
	virtual void setScale(double scale);
	virtual void addToScale(double scale);

	Matrix getTransformMatrix() const { return transformation.getMatrix(); }

	MapObject::ObjectType getType() const { return MapObject::MESH_INSTANCE; }
	
};
