#ifndef PROJECT_OPENGL__CAMERA_H
#define PROJECT_OPENGL__CAMERA_H

#include "math/Point.h"
#include "math/BoundingConvexHull3D.h"
#include "math/Ray.h"
#include "OpenGL.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace OpenGL {

/** A openGL camera
*/
class Camera
{
public:
	enum CameraType {ORTHOGRAPHIC, PERSPECTIVE};
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

	double halfPlaneWidth;
	double halfPlaneHeight;

	double aspect;
	double fieldOfView;
	double nearPlane;
	double farPlane;

	double orthoHeight;

	BoundingConvexHull3D* frustrum;
	GLfloat cameraMatrix [16];

	CameraType cameraType;

	void updateDirections();
	void updateFrustrum();

public:
	Camera(CameraType camera_type = PERSPECTIVE);
	~Camera(void);

	void setPosition(Point pos);
	void translate(Point translation);
	void setLookPosition(Point pos);
	void setLookDirection(Point dir);
	void setUpDirection(Point dir);
	void setAspect(double aspect);
	void setFieldOfViewDegrees(double fov);
	void setNearPlane(double near);
	void setFarPlane(double far);
	void setOrthoHeight(double height);
	
	Point getPosition() const { return cameraPosition; }
	Point getLookPosition() const { return cameraLookPosition; }
	Point getLookDirection() const { return cameraLookDirection; }
	Point getUpDirection() const { return actualCameraUpDirection; }
	Point getRightDirection() const { return cameraRightDirection; }
	double getAspect() const { return aspect; }
	double getFieldOfViewDegrees() const { return fieldOfView; }
	double getNearPlane() const { return nearPlane; }
	double getFarPlane() const { return farPlane; }
	double getOrthoHeight() const { return orthoHeight; }
	Point cameraToWorld(Point p);
	Point cameraToWorld(double x, double y, double z = 1.0);

	void glLookAt();
	void glProjection();

	const BoundingConvexHull3D* getFrustrum() const { return frustrum; }
	const GLfloat* getCameraMatrix() const { return cameraMatrix; }

};

}  // namespace OpenGL
}  // namespace Project

#endif
