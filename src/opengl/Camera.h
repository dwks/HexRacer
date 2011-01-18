#pragma once

#include "math/Point.h"
#include "math/BoundingConvexHull3D.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace OpenGL {

/** A openGL camera
*/
class Camera
{
private:

	Point cameraPosition;
	Point cameraLookPosition;
	Point cameraUpDirection;
	Point cameraRightDirection;
	Point cameraLookDirection;
	Point actualCameraUpDirection;

	Point topPlaneNormal;
	Point leftPlaneNormal;
	Point rightPlaneNormal;
	Point bottomPlaneNormal;

	double aspect;
	double fieldOfView;
	double nearPlane;
	double farPlane;

	BoundingConvexHull3D* frustrum;

	void updateDirections();
	void updateFrustrum();

public:
	Camera(void);
	~Camera(void);

	void setPosition(Point pos, bool keep_focus = false);
	void translate(Point translation, bool keep_focus = false);
	void setLookPosition(Point pos);
	void setLookDirection(Point dir);
	void setUpDirection(Point dir);
	void setAspect(double aspect);
	void setFieldOfViewDegrees(double fov);
	void setNearPlane(double near);
	void setFarPlane(double far);
	
	Point getPosition() { return cameraPosition; }
	Point getLookPosition() { return cameraLookPosition; }
	Point getLookDirection() { return cameraLookDirection; }
	Point getUpDirection() { return cameraUpDirection; }
	Point getRightDirection() { return cameraRightDirection; }
	double getAspect() { return aspect; }
	double getFieldOfViewDegrees() { return fieldOfView; }
	double getNearPlane() { return nearPlane; }
	double getFarPlane() { return farPlane; }

	void glLookAt();
	void glProjection();

	const BoundingConvexHull3D* getFrustrum() { return frustrum; }

};

}  // namespace OpenGL
}  // namespace Project
