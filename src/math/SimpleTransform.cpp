#include "SimpleTransform.h"
#include <float.h>
#include "opengl/OpenGl.h"
#include "opengl/MathWrapper.h"

namespace Project {
namespace Math {

	SimpleTransform::SimpleTransform(Point _translation, double _scale, Point _rotationRadians) {
		translation = _translation;
		scale = _scale;
		rotationRadians = _rotationRadians;

		minScale = DBL_MIN;
		maxScale = DBL_MAX;
	}

	void SimpleTransform::update() {

		vector = Point(1.0, 0.0, 0.0);
		//Yaw
		vector = vector.rotateY(rotationRadians.getX());
		//Pitch
		vector = vector.rotateZ(rotationRadians.getY());
		//Roll
		vector = vector.rotateY(rotationRadians.getZ());
		vector *= scale;

		matrix.setToIdentity();
		matrix = matrix*Matrix::getTranslationMatrix(translation);
		matrix = matrix*Matrix::getScalingMatrix(scale);
		matrix = matrix*Matrix::getRotationMatrix(Y_AXIS, rotationRadians.getCoord(Y_AXIS));
		matrix = matrix*Matrix::getRotationMatrix(X_AXIS, rotationRadians.getCoord(X_AXIS));
		matrix = matrix*Matrix::getRotationMatrix(Z_AXIS, rotationRadians.getCoord(Y_AXIS));
	}

	void SimpleTransform::boundScale() {
		scale = minimum(scale, maxScale);
		scale = maximum(scale, minScale);
	}

	void SimpleTransform::boundRotation() {
		double two_pi = PI*2.0;
		for (int i = 0; i < 3; i++) {
			Axis axis = static_cast<Axis>(i);
			rotationRadians.setCoord(smartMod(rotationRadians.getCoord(axis), two_pi), axis);
		}
	}

	void SimpleTransform::setScale(double _scale) {
		scale = _scale;
		boundScale();
		update();
	}

	void SimpleTransform::addToScale(double delta) {
		scale += delta;
		boundScale();
		update();
	}

	void SimpleTransform::setRotationRadians(const Point& _rotation) {
		rotationRadians = _rotation;
		boundRotation();
		update();
	}

	void SimpleTransform::setRotationDegrees(const Point& _rotation) {
		rotationRadians = _rotation/180.0*PI;
		boundRotation();
		update();
	}

	void SimpleTransform::setRotationRadians(double rotation, RotationAxis axis) {
		rotationRadians.setCoord(rotation, static_cast<Axis>(axis));
		boundRotation();
		update();
	}

	void SimpleTransform::setRotationDegrees(double rotation, RotationAxis axis) {
		rotationRadians.setCoord(degreesToRadians(rotation), static_cast<Axis>(axis));
		boundRotation();
		update();
	}

	void SimpleTransform::rotateRadians(double rotation, RotationAxis axis) {
		rotationRadians.setCoord( rotationRadians.getCoord(static_cast<Axis>(axis))+rotation, static_cast<Axis>(axis) );
		boundRotation();
		update();
	}

	void SimpleTransform::rotateDegrees(double rotation, RotationAxis axis) {
		rotationRadians.setCoord( rotationRadians.getCoord(static_cast<Axis>(axis))+degreesToRadians(rotation), static_cast<Axis>(axis) );
		boundRotation();
		update();
	}

	void SimpleTransform::glApply() {

		OpenGL::MathWrapper::glTranslate(translation);
		//Yaw
		glRotatef(radiansToDegrees(rotationRadians.getX()), 0.0f, 1.0f, 0.0f);
		//Pitch
		glRotatef(radiansToDegrees(rotationRadians.getY()), 0.0f, 0.0f, 1.0f);
		//Roll
		glRotatef(radiansToDegrees(rotationRadians.getZ()), 0.0f, 1.0f, 0.0f);
		glScalef(scale, scale, scale);

	}


}  // namespace Math
}  // namespace Project
