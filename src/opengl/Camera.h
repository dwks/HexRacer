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
	Point givenCameraUpDirection;
	Point cameraRightDirection;
	Point cameraLookDirection;
	Point cameraUpDirection;

	double halfPlaneWidth;
	double halfPlaneHeight;

	double aspect;
	double fieldOfView;
	double nearPlane;
	double farPlane;

	double orthoHeight;

	bool frustrumNearPlane;
	bool frustrumFarPlane;

	bool matrixChanged;

	bool frustrumChanged;
	bool frustrumDimensionsChanged;

	BoundingConvexHull3D* frustrum;
	GLfloat cameraMatrix [16];

	CameraType cameraType;

	void updatePosition();
	void updateDirections();
	void updateDimensions();
	void updateFrustrum();
	void updateMatrix();

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

	void setFrustrumNearPlaneEnabled(bool enabled);
	void setFrustrumFarPlaneEnabled(bool enabled);
	
	Point getPosition() const { return cameraPosition; }
	Point getLookPosition() const { return cameraLookPosition; }
	Point getLookDirection() const { return cameraLookDirection; }
	Point getUpDirection() const { return cameraUpDirection; }
	Point getRightDirection() const { return cameraRightDirection; }
	double getAspect() const { return aspect; }
	double getFieldOfViewDegrees() const { return fieldOfView; }
	double getNearPlane() const { return nearPlane; }
	double getFarPlane() const { return farPlane; }
	double getOrthoHeight() const { return orthoHeight; }
	double getOrthoWidth() const { return orthoHeight*aspect; }
	Point cameraToWorld(Point p) const;
	Point cameraToWorld(double x, double y, double z = 1.0) const;
	Ray cameraRay(double x, double y) const;

	void setCameraType(CameraType type);
	CameraType getCameraType() const { return cameraType; }

	void glLookAt() const;
	void glProjection() const;

	const BoundingConvexHull3D* getFrustrum();
	const GLfloat* getCameraMatrix();

};

}  // namespace OpenGL
}  // namespace Project

#endif
