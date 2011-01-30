#include "MeshInstanceObject.h"

MeshInstanceObject::MeshInstanceObject(TransformedMesh* transformed_mesh)
{
	transformedMesh = transformed_mesh;
	transformation = transformedMesh->getTransformation();
}

void MeshInstanceObject::update() {
	transformedMesh->setTransformation(transformation);
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