#include "MeshInstanceObject.h"
#include "opengl/Material.h"

MeshInstanceObject::MeshInstanceObject(MeshInstance* mesh_instance, TransformedMesh* transformed_mesh)
{
	meshInstance = mesh_instance;
	transformedMesh = transformed_mesh;
	transformation = mesh_instance->getTransformation();
	transformed_mesh->setTransformation(transformation);
	updateTint();
}

MeshInstanceObject::~MeshInstanceObject() {
	if (transformedMesh->getRenderProperties()->hasMaterialTint())
		delete transformedMesh->getRenderProperties()->getMaterialTint();
	delete(transformedMesh);
}

void MeshInstanceObject::updateTransformation() {
	transformedMesh->setTransformation(transformation);
	meshInstance->setTransformation(transformation);
}
void MeshInstanceObject::updateTint() {

	if (meshInstance->hasTint()) {

		if (!transformedMesh->getRenderProperties()->hasMaterialTint())
			transformedMesh->getRenderProperties()->setMaterialTint(new OpenGL::Material("tint"));

		OpenGL::Material* tint = transformedMesh->getRenderProperties()->getMaterialTint();
		tint->setDiffuse(meshInstance->getDiffuseTint());
		tint->setSpecular(meshInstance->getSpecularTint());
		tint->setAmbient(meshInstance->getAmbientTint());
	}
	else {
		transformedMesh->getRenderProperties()->clearMaterialTint();
	}
}

void MeshInstanceObject::setPosition(const Point& new_position) {
	transformation.setTranslation(new_position);
	updateTransformation();
}

void MeshInstanceObject::translate(const Point& translation) {
	transformation.translate(translation);
	updateTransformation();
}

void MeshInstanceObject::setRotation(double radians, RotationAxis axis) {
	transformation.setRotationRadians(radians, axis);
	updateTransformation();
}

BoundingBox3D MeshInstanceObject::getBoundingBox() const {
	return BoundingBox3D(transformedMesh->getMeshGroup()->getBoundingBox());
}


void MeshInstanceObject::setScale(double scale) {
	transformation.setScale(scale);
	updateTransformation();
}

void MeshInstanceObject::addToScale(double scale) {
	transformation.addToScale(scale);
	updateTransformation();
}

void MeshInstanceObject::setDiffuse(const Color& color) {
	meshInstance->setDiffuseTint(color);
	updateTint();
}
void MeshInstanceObject::setSpecular(const Color& color) {
	meshInstance->setSpecularTint(color);
	updateTint();
}
void MeshInstanceObject::setAmbient(const Color& color) {
	meshInstance->setAmbientTint(color);
	updateTint();
}