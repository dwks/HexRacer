#include "Camera.h"
#include "opengl/OpenGL.h"
using namespace Project;
using namespace OpenGL;

Camera::Camera(void)
{
	setAspect(1.0f);
	setFieldOfViewDegrees(90.0f);
	setNearPlane(0.01f);
	setFarPlane(20.0f);
	setUpDirection(Point(0.0f, 1.0f, 0.0f));
}

Camera::~Camera(void)
{
}

void Camera::setPosition(Point pos, bool keep_focus) {
	cameraPosition = pos;
	if (keep_focus)
		updateDirections();
	else
		cameraLookPosition = cameraPosition+cameraLookDirection;
		
}

void Camera::translate(Point translation, bool keep_focus) {
	cameraPosition += translation;
	if (keep_focus)
		updateDirections();
	else
		cameraLookPosition += translation;
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
	cameraRightDirection = cameraLookDirection.crossProduct(cameraUpDirection);
}

void Camera::setAspect(double aspect) {
	if (aspect > 0.0f)
		this->aspect = aspect;
}
void Camera::setFieldOfViewDegrees(double fov) {
	if (fov > 0.0f)
		this->fieldOfView = fov;
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