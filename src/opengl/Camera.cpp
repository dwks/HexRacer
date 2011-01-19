#include "Camera.h"
#include "OpenGL.h"
#include "math/Values.h"
#include "math/Geometry.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

Camera::Camera(void)
{
	frustrum = new BoundingConvexHull3D(4);
	setAspect(1.0f);
	setFieldOfViewDegrees(90.0f);
	setNearPlane(0.01f);
	setFarPlane(20.0f);
	setUpDirection(Point(0.0f, 1.0f, 0.0f));
}

Camera::~Camera(void)
{
}

void Camera::setPosition(Point pos) {
	cameraPosition = pos;
	cameraLookPosition = cameraPosition+cameraLookDirection;
	updateFrustrum();
		
}

void Camera::translate(Point translation) {
	setPosition(cameraPosition + translation);
}

void Camera::setLookPosition(Point pos) {
	cameraLookPosition = pos;
	updateDirections();
}

void Camera::setLookDirection(Point dir) {
	setLookPosition(cameraPosition+dir);
}

void Camera::setUpDirection(Point dir) {
	cameraUpDirection = dir.normalized();
	updateDirections();
}

void Camera::updateDirections() {

	cameraLookDirection = (cameraLookPosition-cameraPosition).normalized();
	cameraRightDirection = cameraLookDirection.crossProduct(cameraUpDirection).normalized();
	actualCameraUpDirection = cameraRightDirection.crossProduct(cameraLookDirection);

	//Update the plane normals
	float angle = fieldOfView/360.0f*PI;
	float half_plane_height = tan(angle);//*2.0f;
	float half_plane_width = half_plane_height*aspect;

	Point center = cameraPosition+cameraLookDirection;
	Point top_left = center-(cameraRightDirection*half_plane_width)+(actualCameraUpDirection*half_plane_height);
	Point top_right = center+(cameraRightDirection*half_plane_width)+(actualCameraUpDirection*half_plane_height);
	Point bottom_left = center-(cameraRightDirection*half_plane_width)-(actualCameraUpDirection*half_plane_height);
	Point bottom_right = center+(cameraRightDirection*half_plane_width)-(actualCameraUpDirection*half_plane_height);

	leftPlaneNormal = Geometry::triangleNormal(cameraPosition, bottom_left, top_left);
	rightPlaneNormal = Geometry::triangleNormal(cameraPosition, top_right, bottom_right);
	topPlaneNormal = Geometry::triangleNormal(cameraPosition, top_left, top_right);
	bottomPlaneNormal = Geometry::triangleNormal(cameraPosition, bottom_right, bottom_left);

	updateFrustrum();
}

void Camera::setAspect(double aspect) {
	if (aspect > 0.0f) {
		this->aspect = aspect;
		updateDirections();
	}
}
void Camera::setFieldOfViewDegrees(double fov) {
	if (fov > 0.0f) {
		this->fieldOfView = fov;
		updateDirections();
	}
}

void Camera::setNearPlane(double near_plane) {
	nearPlane = near_plane;
}


void Camera::setFarPlane(double far_plane) {
	farPlane = far_plane;
}

void Camera::glLookAt() {
	gluLookAt(
		cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ(),
		cameraLookPosition.getX(), cameraLookPosition.getY(), cameraLookPosition.getZ(),
		cameraUpDirection.getX(), cameraUpDirection.getY(), cameraUpDirection.getZ());
}

void Camera::glProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspect, nearPlane, farPlane);
}

void Camera::updateFrustrum() {
	frustrum->setPlaneOrigin(0, cameraPosition);
	frustrum->setPlaneNormal(0, rightPlaneNormal);
	frustrum->setPlaneOrigin(1, cameraPosition);
	frustrum->setPlaneNormal(1, leftPlaneNormal);
	frustrum->setPlaneOrigin(2, cameraPosition);
	frustrum->setPlaneNormal(2, topPlaneNormal);
	frustrum->setPlaneOrigin(3, cameraPosition);
	frustrum->setPlaneNormal(3, bottomPlaneNormal);
}