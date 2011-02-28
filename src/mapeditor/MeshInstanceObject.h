#pragma once

#include "MapObject.h"
#include "mesh/TransformedMesh.h"
#include "math/SimpleTransform.h"
#include "map/MeshInstance.h"
using namespace Project;
using namespace Render;
using namespace Math;
using namespace Map;
using namespace Mesh;

class MeshInstanceObject
	: public MapObject
{
private:

	MeshInstance* meshInstance;
	TransformedMesh* transformedMesh;
	SimpleTransform transformation;
	void updateTransformation();
	void updateTint();

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

	bool hasColors() const { return true; }
	void setDiffuse(const Color& color);
	void setSpecular(const Color& color);
	void setAmbient(const Color& color);
	Color getDiffuse() const { return meshInstance->getDiffuseTint(); }
	Color getSpecular() const { return meshInstance->getSpecularTint(); }
	Color getAmbient() const { return meshInstance->getAmbientTint(); }

	Matrix getTransformMatrix() const { return transformation.getMatrix(); }

	MapObject::ObjectType getType() const { return MapObject::MESH_INSTANCE; }
	
};
