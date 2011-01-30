#ifndef PROJECT_MATH__SIMPLE_TRANSFORM_H
#define PROJECT_MATH__SIMPLE_TRANSFORM_H

#include "Point.h"
#include "Matrix.h"
#include "Values.h"
#include "RotationAxis.h"

namespace Project {
namespace Math {

class SimpleTransform {
private:

	Point translation;
	double scale;
	Point rotationRadians;

	double minScale;
	double maxScale;

	Point vector;
	Matrix matrix;

	void update();
	void boundScale();
	void boundRotation();

public:

	SimpleTransform(Point _translation = Point(),
		double _scale = 1.0,
		Point _rotationRadians = Point());

	Point getTranslation() const { return translation; }
	double getScale() const { return scale; }
	Point getRotationRadians() const { return rotationRadians; }
	Point getRotationDegrees() const { return rotationRadians/PI*180.0; }
	double getRotationRadians(RotationAxis axis) const { return rotationRadians.getCoord(static_cast<Axis>(axis)); }
	double getRotationDegrees(RotationAxis axis) const { return radiansToDegrees(rotationRadians.getCoord(static_cast<Axis>(axis))); }

	Point getVector() const { return vector; }
	
	Matrix getMatrix() const { return matrix; }

	void setMinScale(double min_scale);

	void setTranslation(const Point& _translation) { translation = _translation; update(); }
	void translate(const Point& _translation) { translation += _translation; update(); }

	void setScale(double _scale);
	void addToScale(double delta);

	void setRotationRadians(const Point& _rotation);
	void setRotationDegrees(const Point& _rotation);
	void setRotationRadians(double rotation, RotationAxis axis);
	void setRotationDegrees(double rotation, RotationAxis axis);
	void rotateRadians(double rotation, RotationAxis axis);
	void rotateDegrees(double rotation, RotationAxis axis);

	void glApply() const;


};

}  // namespace Math
}  // namespace Project

#endif
