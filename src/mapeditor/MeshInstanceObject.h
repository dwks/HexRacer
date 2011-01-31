#pragma once

#include "MapObject.h"
#include "render/TransformedMesh.h"
#include "math/SimpleTransform.h"
#include "map/MeshInstance.h"
using namespace Project;
using namespace Render;
using namespace Math;
using namespace Map;

class MeshInstanceObject
	: public MapObject
{
private:

	MeshInstance* meshInstance;
	TransformedMesh* transformedMesh;
	SimpleTransform transformation;
	void update();

public:

	MeshInstanceObject(MeshInstance* mesh_instance, TransformedMesh* transformed_mesh);
	~MeshInstanceObject();

	MeshInstance* getMeshInstance() { return meshInstance; }
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
