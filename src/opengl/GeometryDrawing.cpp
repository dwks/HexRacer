#include "GeometryDrawing.h"
#include "MathWrapper.h"
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

}
void GeometryDrawing::drawBoundingBox3D(BoundingBox3D& object, bool wireframe) {

	glBegin(GL_LINE_STRIP);

	for (int i = 0; i < 4; i++)
		MathWrapper::glVertex(object.getCorner(i));
	MathWrapper::glVertex(object.getCorner(0));
	for (int i = 4; i < 8; i++)
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
void GeometryDrawing::drawBoundingBox2D(BoundingBox2D& object, bool wireframe) {

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 4; i++) {
		MathWrapper::glVertex(object.getCorner(i));
	}
	glEnd();
}