#include "GeometryDrawing.h"
#include "MathWrapper.h"
#include "OpenGL.h"
#include <typeinfo>
using namespace Project;
using namespace OpenGL;
using namespace Math;

GeometryDrawing::GeometryDrawing(void)
{
}

GeometryDrawing::~GeometryDrawing(void)
{
}

void GeometryDrawing::drawObject(ObjectSpatial& object, bool wireframe) {

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
	else if (object_type == typeid(Triangle3D&)) {
		drawTriangle3D((Triangle3D&) object, wireframe);
		return;
	}


	/*
    
	BoundingBox3D* bb3D = dynamic_cast<BoundingBox3D*>(&object);
	if (bb3D) {
		drawBoundingBox3D(*bb3D, wireframe);
		return;
	}
	BoundingBox2D* bb2D = dynamic_cast<BoundingBox2D*>(&object);
	if (bb2D) {
		drawBoundingBox2D(*bb2D, wireframe);
		return;
	}
	BoundingTriangle2D* bt2D = dynamic_cast<BoundingTriangle2D*>(&object);
	if (bt2D) {
		drawBoundingTriangle2D(*bt2D, wireframe);
		return;
	}

	Triangle3D* t3D = dynamic_cast<Triangle3D*>(&object);
	if (t3D) {
		drawTriangle3D(*t3D, wireframe);
		return;
	}

	*/

}

void GeometryDrawing::drawBoundingObject(Math::BoundingObject& object, bool wireframe) {

	BoundingObject3D* obj_3D = dynamic_cast<BoundingObject3D*>(&object);
	if (obj_3D) {
		drawObject(*obj_3D, wireframe);
		return;
	}
	BoundingObject2D* obj_2D = dynamic_cast<BoundingObject2D*>(&object);
	if (obj_2D) {
		drawObject(*obj_2D, wireframe);
		return;
	}

}

void GeometryDrawing::drawBoundingBox3D(BoundingBox3D& object, bool wireframe) {

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

void GeometryDrawing::drawBoundingBox2D(BoundingBox2D& object, bool wireframe) {

	if (wireframe)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_QUADS);

	for (unsigned int i = 0; i < 4; i++) {
		MathWrapper::glVertex(object.getCorner(i));
	}
	glEnd();
}

void GeometryDrawing::drawBoundingTriangle2D(Math::BoundingTriangle2D& object, bool wireframe) {

	if (wireframe)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < 3; i++) {
		MathWrapper::glVertex(object.getVertex(i));
	}
	glEnd();

}

void GeometryDrawing::drawTriangle3D(Math::Triangle3D& object, bool wireframe) {

	if (wireframe)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < 3; i++) {
		MathWrapper::glVertex(object.getVertex(i));
	}
	glEnd();

}
