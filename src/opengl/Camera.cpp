#include "Camera.h"
#include "math/Values.h"
#include "math/Geometry.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

Camera::Camera(CameraType camera_type)
{

	cameraMatrix[3] = 0.0f;
	cameraMatrix[7] = 0.0f;
	cameraMatrix[11] = 0.0f;
	cameraMatrix[15] = 0.0f;

	cameraType = camera_type;

	frustrum = new BoundingConvexHull3D(4);
	setAspect(1.0f);
	setFieldOfViewDegrees(90.0f);
	setNearPlane(0.01f);
	setFarPlane(20.0f);
	setUpDirection(Point(0.0f, 1.0f, 0.0f));

	frustrumNearPlane = false;
	frustrumFarPlane = false;
}

Camera::~Camera(void)
{
	//delete(frustrum);
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
	givenCameraUpDirection = dir.normalized();
	updateDirections();
}

void Camera::updateDirections() {

	cameraLookDirection = (cameraLookPosition-cameraPosition).normalized();
	cameraRightDirection = cameraLookDirection.crossProduct(givenCameraUpDirection).normalized();
	cameraUpDirection = cameraRightDirection.crossProduct(cameraLookDirection);

	//Update the camera matrix

	cameraMatrix[0] = static_cast<GLfloat>(cameraRightDirection.getX());
	cameraMatrix[1] = static_cast<GLfloat>(cameraRightDirection.getY());
	cameraMatrix[2] = static_cast<GLfloat>(cameraRightDirection.getZ());
	
	cameraMatrix[4] = static_cast<GLfloat>(cameraUpDirection.getX());
	cameraMatrix[5] = static_cast<GLfloat>(cameraUpDirection.getY());
	cameraMatrix[6] = static_cast<GLfloat>(cameraUpDirection.getZ());

	cameraMatrix[8] = static_cast<GLfloat>(cameraLookDirection.getX());
	cameraMatrix[9] = static_cast<GLfloat>(cameraLookDirection.getY());
	cameraMatrix[10] = static_cast<GLfloat>(cameraLookDirection.getZ());

	if (cameraType == PERSPECTIVE) {

		//Update the plane normals
		double angle = fieldOfView/360.0*PI;
		halfPlaneHeight = tan(angle);
		halfPlaneWidth = halfPlaneHeight*aspect;

		Point center = cameraPosition+cameraLookDirection;
		Point top_left = center-(cameraRightDirection*halfPlaneWidth)+(cameraUpDirection*halfPlaneHeight);
		Point top_right = center+(cameraRightDirection*halfPlaneWidth)+(cameraUpDirection*halfPlaneHeight);
		Point bottom_left = center-(cameraRightDirection*halfPlaneWidth)-(cameraUpDirection*halfPlaneHeight);
		Point bottom_right = center+(cameraRightDirection*halfPlaneWidth)-(cameraUpDirection*halfPlaneHeight);

		leftPlaneNormal = Geometry::triangleNormal(cameraPosition, bottom_left, top_left);
		rightPlaneNormal = Geometry::triangleNormal(cameraPosition, top_right, bottom_right);
		topPlaneNormal = Geometry::triangleNormal(cameraPosition, top_left, top_right);
		bottomPlaneNormal = Geometry::triangleNormal(cameraPosition, bottom_right, bottom_left);

	}
	else {
		leftPlaneNormal = cameraRightDirection;
		rightPlaneNormal = cameraRightDirection*(-1.0);
		bottomPlaneNormal = cameraUpDirection;
		topPlaneNormal = cameraUpDirection*(-1.0);
	}

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
	updateFrustrum();
}


void Camera::setFarPlane(double far_plane) {
	farPlane = far_plane;
	updateFrustrum();
}

void Camera::glLookAt() {
	gluLookAt(
		cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ(),
		cameraLookPosition.getX(), cameraLookPosition.getY(), cameraLookPosition.getZ(),
		givenCameraUpDirection.getX(), givenCameraUpDirection.getY(), givenCameraUpDirection.getZ());
}

void Camera::glProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (cameraType == PERSPECTIVE)
		gluPerspective(fieldOfView, aspect, nearPlane, farPlane);
	else {
		double half_height = orthoHeight*0.5;
		double half_width = half_height*aspect;
		glOrtho(-half_width, half_width, -half_height, half_height, nearPlane, farPlane);
	}
}

void Camera::updateFrustrum() {

	
	frustrum->setNumPlanes(4 + (int)frustrumNearPlane + (int)frustrumFarPlane);


	if (cameraType == PERSPECTIVE) {

		frustrum->setPlaneOrigin(0, cameraPosition);
		frustrum->setPlaneNormal(0, rightPlaneNormal);
		frustrum->setPlaneOrigin(1, cameraPosition);
		frustrum->setPlaneNormal(1, leftPlaneNormal);
		frustrum->setPlaneOrigin(2, cameraPosition);
		frustrum->setPlaneNormal(2, topPlaneNormal);
		frustrum->setPlaneOrigin(3, cameraPosition);
		frustrum->setPlaneNormal(3, bottomPlaneNormal);

	}
	else {

		Point rightEdge = cameraRightDirection*halfPlaneWidth;
		Point topEdge = cameraUpDirection*halfPlaneHeight;
		frustrum->setPlaneOrigin(0, cameraPosition+rightEdge);
		frustrum->setPlaneNormal(0, rightPlaneNormal);
		frustrum->setPlaneOrigin(1, cameraPosition-rightEdge);
		frustrum->setPlaneNormal(1, leftPlaneNormal);
		frustrum->setPlaneOrigin(2, cameraPosition+topEdge);
		frustrum->setPlaneNormal(2, topPlaneNormal);
		frustrum->setPlaneOrigin(3, cameraPosition-topEdge);
		frustrum->setPlaneNormal(3, bottomPlaneNormal);

	}

	int current_plane = 4;

	if (frustrumNearPlane) {
		//Set near plane
		frustrum->setPlaneOrigin(current_plane, cameraPosition+cameraLookDirection*nearPlane);
		frustrum->setPlaneNormal(current_plane, cameraLookDirection);
		current_plane++;
	}
	if (frustrumFarPlane) {
		//Set far plane
		frustrum->setPlaneOrigin(current_plane, cameraPosition+cameraLookDirection*farPlane);
		frustrum->setPlaneNormal(current_plane, cameraLookDirection*(-1.0));
	}

	cameraMatrix[12] = static_cast<GLfloat>(cameraPosition.getX());
	cameraMatrix[13] = static_cast<GLfloat>(cameraPosition.getY());
	cameraMatrix[14] = static_cast<GLfloat>(cameraPosition.getZ());

}

Point Camera::cameraToWorld(Point p) {
	return cameraToWorld(p.getX(), p.getY(), p.getZ());
}

Point Camera::cameraToWorld(double x, double y, double z) {

	x = (x-0.5) * 2.0;
	y = (y-0.5) * 2.0;

	if (cameraType == PERSPECTIVE) {
		x *= z;
		y *= z;
	}

	return (cameraPosition+cameraLookDirection*z
		+(cameraRightDirection*halfPlaneWidth*x)
		+(cameraUpDirection*halfPlaneHeight*y));
}

void Camera::setOrthoHeight(double height) {
	orthoHeight = height;
	halfPlaneHeight = orthoHeight*0.5;
	halfPlaneWidth = halfPlaneHeight*aspect;

	if (cameraType == ORTHOGRAPHIC) {
		updateFrustrum();
	}
}

void Camera::setCameraType(CameraType type) {
	cameraType = type;
	updateDirections();
}

Ray Camera::cameraRay(double x, double y) {

	Point plane_point = cameraToWorld(x, y, 1.0);

	if (cameraType == ORTHOGRAPHIC) {
		Point near_plane_point = cameraToWorld(x, y, 0.0);
		return Ray(near_plane_point, plane_point-near_plane_point, 0.0);
	}
	else {
		return Ray(cameraPosition, plane_point-cameraPosition, 0.0);
	}

}

void Camera::setFrustrumNearPlaneEnabled(bool enabled) {
	frustrumNearPlane = enabled;
	updateFrustrum();
}

void Camera::setFrustrumFarPlaneEnabled(bool enabled) {
	frustrumFarPlane = enabled;
	updateFrustrum();
}