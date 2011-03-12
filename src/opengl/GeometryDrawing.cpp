#include "GeometryDrawing.h"
#include "MathWrapper.h"
#include "OpenGL.h"
#include "math/Vertex2D.h"
#include <typeinfo>
#include <vector>
#include <math.h>
#include "math/Values.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;
using namespace std;

#ifndef SQUARE_ROOT_THREE
#define SQUARE_ROOT_THREE 1.7320508075688772935274463415058723669428052538103806280558
#endif

GeometryDrawing::GeometryDrawing(void)
{
}

GeometryDrawing::~GeometryDrawing(void)
{
}

void GeometryDrawing::drawObject(const ObjectSpatial& object, bool wireframe) {

	const std::type_info& object_type = typeid(object);

	if (object_type == typeid(BoundingBox3D&)) {
		drawBoundingBox3D((BoundingBox3D&) object, wireframe);
		return;
	}
	else if (object_type == typeid(BoundingBox2D&)) {
		drawBoundingBox2D((BoundingBox2D&) object, wireframe);
		return;
	}
	else if (object_type == typeid(BoundingTriangle2D&)) {
		drawBoundingTriangle2D((BoundingTriangle2D&) object, wireframe);
		return;
	}
	else if (object_type == typeid(BoundingCircle&)) {
		drawBoundingCircle((BoundingCircle&) object, wireframe);
		return;
	}
	else if (object_type == typeid(Triangle3D&)) {
		drawTriangle3D((Triangle3D&) object, wireframe);
		return;
	}
	else if (object_type == typeid(BoundingPlane3D&)) {
		drawBoundingPlane3D((BoundingPlane3D&) object, wireframe);
		return;
	}
	else if (object_type == typeid(BoundingConvexHull3D&)) {
		drawBoundingConvexHull3D((BoundingConvexHull3D&) object, wireframe);
		return;
	}
	else if (object_type == typeid(BoundingSphere&)) {
		drawBoundingSphere((BoundingSphere&) object, wireframe);
		return;
	}
	else if (object_type == typeid(Vertex2D&)) {
		glBegin(GL_POINTS);
		MathWrapper::glVertex(((Vertex2D&)object).getPosition());
		glEnd();
		return;
	}

}

void GeometryDrawing::drawBoundingObject(const Math::BoundingObject& object, bool wireframe) {
	drawObject(object.toConstObjectSpatial(), wireframe);
}

void GeometryDrawing::drawBoundingBox3D(const BoundingBox3D& object, bool wireframe) {

	if (wireframe) {

		glBegin(GL_LINE_STRIP);

		for (unsigned int i = 0; i < 4; i++)
			MathWrapper::glVertex(object.getCorner(i));
		MathWrapper::glVertex(object.getCorner(0));
		for (unsigned int i = 4; i < 8; i++)
			MathWrapper::glVertex(object.getCorner(i));
		MathWrapper::glVertex(object.getCorner(4));
		glEnd();

		glBegin(GL_LINES);

		MathWrapper::glVertex(object.getCorner(1));
		MathWrapper::glVertex(object.getCorner(5));

		MathWrapper::glVertex(object.getCorner(2));
		MathWrapper::glVertex(object.getCorner(6));

		MathWrapper::glVertex(object.getCorner(3));
		MathWrapper::glVertex(object.getCorner(7));
		glEnd();

	}
	else {

		glBegin(GL_QUADS);

		//Top face
		glNormal3f(0.0f, 1.0f, 0.0f);
		MathWrapper::glVertex(object.getCorner(false, true, true));
		MathWrapper::glVertex(object.getCorner(true, true, true));
		MathWrapper::glVertex(object.getCorner(true, true, false));
		MathWrapper::glVertex(object.getCorner(false, true, false));
		
		//Bottom face
		glNormal3f(0.0f, -1.0f, 0.0f);
		MathWrapper::glVertex(object.getCorner(false, false, true));
		MathWrapper::glVertex(object.getCorner(true, false, true));
		MathWrapper::glVertex(object.getCorner(true, false, false));
		MathWrapper::glVertex(object.getCorner(false, false, false));

		//Front face
		glNormal3f(0.0f, 0.0f, -1.0f);
		MathWrapper::glVertex(object.getCorner(false, true, false));
		MathWrapper::glVertex(object.getCorner(true, true, false));
		MathWrapper::glVertex(object.getCorner(true, false, false));
		MathWrapper::glVertex(object.getCorner(false, false, false));

		//Back face
		glNormal3f(0.0f, 0.0f, 1.0f);
		MathWrapper::glVertex(object.getCorner(false, true, true));
		MathWrapper::glVertex(object.getCorner(true, true, true));
		MathWrapper::glVertex(object.getCorner(true, false, true));
		MathWrapper::glVertex(object.getCorner(false, false, true));

		//Left face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		MathWrapper::glVertex(object.getCorner(false, false, true));
		MathWrapper::glVertex(object.getCorner(false, true, true));
		MathWrapper::glVertex(object.getCorner(false, true, false));
		MathWrapper::glVertex(object.getCorner(false, false, false));

		//Right face
		glNormal3f(1.0f, 0.0f, 0.0f);
		MathWrapper::glVertex(object.getCorner(true, false, true));
		MathWrapper::glVertex(object.getCorner(true, true, true));
		MathWrapper::glVertex(object.getCorner(true, true, false));
		MathWrapper::glVertex(object.getCorner(true, false, false));

		glEnd();
	}

}

void GeometryDrawing::drawBoundingBox2D(const BoundingBox2D& object, bool wireframe) {

	if (wireframe)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_QUADS);

	for (unsigned int i = 0; i < 4; i++) {
		MathWrapper::glVertex(object.getCorner(i));
	}
	glEnd();
}

void GeometryDrawing::drawBoundingCircle(const Math::BoundingCircle& object, bool wireframe) {

	if (wireframe)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_POLYGON);

	Point p = object.centroid();
	double r = object.getRadius();
	Axis axis = object.getProjectAxis();

	for (unsigned int i = 0; i < 36; i++) {
		double u = (double) i / 18.0 * PI;
		MathWrapper::glVertex(Point::point2D(
            (p.getU(axis) + cos(u)) * r,
            (p.getV(axis) + sin(u)) * r,
            axis));
	}

	glEnd();
}
void GeometryDrawing::drawBoundingTriangle2D(const Math::BoundingTriangle2D& object, bool wireframe) {

	if (wireframe)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < 3; i++) {
		MathWrapper::glVertex(object.getVertex(i));
	}
	glEnd();

}

void GeometryDrawing::drawTriangle3D(const Math::Triangle3D& object, bool wireframe) {

	if (wireframe)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < 3; i++) {
		MathWrapper::glVertex(object.getVertex(i));
	}
	glEnd();

}

void GeometryDrawing::drawBoundingPlane3D(const Math::BoundingPlane3D& object, bool wireframe) {
	//Get the plane's tangent and bitangent vector
	Point tangent;
	Point bitangent;
	if (object.getNormal().getX() == 0.0 && object.getNormal().getZ() == 0.0)
		tangent = object.getNormal().rotate90CW(X_AXIS).normalized();
	else
		tangent = object.getNormal().rotate90CW(Y_AXIS).normalized();
	bitangent = tangent.crossProduct(object.getNormal()).normalized();
	tangent = bitangent.crossProduct(object.getNormal()).normalized();

	float plane_size = 20.0f;
	Point offset_u = tangent*plane_size;
	Point offset_v = bitangent*plane_size;
	Point origin = object.centroid();
	if (wireframe) {
		glBegin(GL_LINES);

		MathWrapper::glVertex(origin);
		MathWrapper::glVertex(origin+object.getNormal());

		for (float u = -1.0f; u <= 1.0f; u += 0.1f) {
			MathWrapper::glVertex(origin-offset_v+offset_u*u);
			MathWrapper::glVertex(origin+offset_v+offset_u*u);
		}
		for (float v = -1.0f; v <= 1.0f; v += 0.1f) {
			MathWrapper::glVertex(origin-offset_u+offset_v*v);
			MathWrapper::glVertex(origin+offset_u+offset_v*v);
		}
		glEnd();
	}
	else {
		glBegin(GL_QUADS);
		MathWrapper::glNormal(object.getNormal());
		MathWrapper::glVertex(origin-offset_u-offset_v);
		MathWrapper::glVertex(origin-offset_u+offset_v);
		MathWrapper::glVertex(origin+offset_u+offset_v);
		MathWrapper::glVertex(origin+offset_u-offset_v);
		glEnd();
	}
}

void GeometryDrawing::drawBoundingConvexHull3D(const Math::BoundingConvexHull3D& object, bool wireframe) {
	vector<BoundingPlane3D> planes = object.getPlanes();
	for (unsigned int i = 0; i < planes.size(); i++)
		drawBoundingPlane3D(planes[i], wireframe);
}

void GeometryDrawing::drawBoundingSphere(const Math::BoundingSphere& object, bool wireframe) {
	GLUquadric* quad = gluNewQuadric();
	if (wireframe)
		gluQuadricDrawStyle(quad, GLU_LINE);
	glPushMatrix();
	glTranslatef(object.getPosition().getX(), object.getPosition().getY(), object.getPosition().getZ());
	gluSphere(quad, object.getRadius(), 12, 12);
	glPopMatrix();
	gluDeleteQuadric(quad);
}

void GeometryDrawing::drawHexagon(const Math::Vertex2D& center, double radius, bool edge_only) {

	Math::Point center_point = center.getPosition();

	if (!edge_only) {
		glBegin(GL_TRIANGLE_FAN);
		MathWrapper::glVertex(center_point);
	}
	else
		glBegin(GL_LINE_LOOP);

	Math::Point first_vert;

	for (short i = 0; i < 6; i++) {

		Math::Point vert;

		switch (i) {
			case 0:
				vert = center_point+Point::point2D(radius, 0.0, center.getProjectAxis());
				first_vert = vert; break;
			case 1:
				vert = center_point+Point::point2D(radius*0.5, radius*SQUARE_ROOT_THREE*0.5, center.getProjectAxis());
				break;
			case 2:
				vert = center_point+Point::point2D(-radius*0.5, radius*SQUARE_ROOT_THREE*0.5, center.getProjectAxis());
				break;
			case 3:
				vert = center_point+Point::point2D(-radius, 0.0, center.getProjectAxis());
				break;
			case 4:
				vert = center_point+Point::point2D(-radius*0.5, -radius*SQUARE_ROOT_THREE*0.5, center.getProjectAxis());
				break;
			case 5:
				vert = center_point+Point::point2D(radius*0.5, -radius*SQUARE_ROOT_THREE*0.5, center.getProjectAxis());
				break;
			default:
				vert = center_point;
				break;
		}

		MathWrapper::glVertex(vert);
	}

	if (!edge_only)
		MathWrapper::glVertex(first_vert);

	glEnd();
		
}