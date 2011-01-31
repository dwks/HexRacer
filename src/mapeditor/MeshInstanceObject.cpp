#include "MeshInstanceObject.h"

MeshInstanceObject::MeshInstanceObject(MeshInstance* mesh_instance, TransformedMesh* transformed_mesh)
{
	meshInstance = mesh_instance;
	transformedMesh = transformed_mesh;
	transformation = mesh_instance->getTransformation();
	transformed_mesh->setTransformation(transformation);
}

MeshInstanceObject::~MeshInstanceObject() {
	delete(transformedMesh);
}

void MeshInstanceObject::update() {
	transformedMesh->setTransformation(transformation);
	meshInstance->setTransformation(transformation);
}

void MeshInstanceObject::setPosition(const Point& new_position) {
	transformation.setTranslation(new_position);
	update();
}

void MeshInstanceObject::translate(const Point& translation) {
	transformation.translate(translation);
	update();
}

void MeshInstanceObject::setRotation(double radians, RotationAxis axis) {
	transformation.setRotationRadians(radians, axis);
	update();
}

BoundingBox3D MeshInstanceObject::getBoundingBox() const {
	return BoundingBox3D(transformedMesh->getMeshGroup()->getBoundingBox());
}

void MeshInstanceObject::setScale(double scale) {
	transformation.setScale(scale);
	update();
}

void MeshInstanceObject::addToScale(double scale) {
	transformation.addToScale(scale);
	update();
}