#include "SimpleTransform.h"
#include <float.h>
#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"
#include <istream>
#include <ostream>

namespace Project {
namespace Math {

	SimpleTransform::SimpleTransform(Point _translation, double _scale, Point _rotationRadians) {
		translation = _translation;
		scale = _scale;
		rotationRadians = _rotationRadians;

		minScale = DBL_MIN;
		maxScale = DBL_MAX;

		update();
	}

	void SimpleTransform::update() {

		vector = Point(0.0, 0.0, -1.0);
		//Yaw
		vector = vector.rotateY(rotationRadians.getX());
		//Pitch
		vector = vector.rotateX(rotationRadians.getY());
		//Roll
		vector = vector.rotateZ(rotationRadians.getZ());
		vector *= scale;

		matrix.setToIdentity();
		matrix = matrix*Matrix::getTranslationMatrix(translation);
		matrix = matrix*Matrix::getScalingMatrix(scale);
		//Yaw
		matrix = matrix*Matrix::getRotationMatrix(Y_AXIS, rotationRadians.getCoord(X_AXIS));
		//Pitch
		matrix = matrix*Matrix::getRotationMatrix(X_AXIS, rotationRadians.getCoord(Y_AXIS));
		//Roll
		matrix = matrix*Matrix::getRotationMatrix(Z_AXIS, rotationRadians.getCoord(Z_AXIS));
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

	void SimpleTransform::glApply() const {

		OpenGL::MathWrapper::glTranslate(translation);
		//Yaw
		glRotated(radiansToDegrees(rotationRadians.getX()), 0.0, 1.0, 0.0);
		//Pitch
		glRotated(radiansToDegrees(rotationRadians.getY()), 1.0, 0.0, 0.0);
		//Roll
		glRotated(radiansToDegrees(rotationRadians.getZ()), 0.0, 0.0, 1.0);
		glScaled(scale, scale, scale);

	}

	std::ostream &operator << (std::ostream &stream, const SimpleTransform &transform) {
		stream << transform.getTranslation() << ' ' << transform.getScale() << ' ' << transform.getRotationRadians();
		return stream;
	}

	std::istream &operator >> (std::istream &stream, SimpleTransform &transform) {
		Point translation;
		double scale;
		Point rotation;
		stream >> translation >> scale >> rotation;
		transform = SimpleTransform(translation, scale, rotation);
		return stream;
	}

}  // namespace Math
}  // namespace Project
