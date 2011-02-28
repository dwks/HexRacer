#include "PaintCell.h"
#include "math/Geometry.h"
#include "math/Values.h"
#include "opengl/MathWrapper.h"
#include <ostream>
#include <istream>
using namespace Project;
using namespace Math;

#ifndef SQUARE_ROOT_THREE
#define SQUARE_ROOT_THREE 1.7320508075688772935274463415058723669428052538103806280558
#endif

namespace Project {
namespace Paint {

	PaintCell::PaintCell(Point _center) {
		index = -1;
		playerColor = -1;
		center = _center;
		displayList = 0;
		normal = NULL;
		vertex = new Math::Point* [CELL_VERTICES];
		for (int i = 0; i < CELL_VERTICES; i++) {
			vertex[i] = NULL;
		}
	}

	PaintCell::PaintCell(Math::Point _center, Math::Point* vertices, Math::Point _normal, int _index) {
		playerColor = -1;
		center = _center;
		displayList = 0;
		for (int i = 0; i < CELL_VERTICES; i++) {
			vertex[i] = new Math::Point(vertices[i]);
		}
		normal = new Math::Point(_normal);
		index = _index;
		setBoundingDimensions();
	}

	void PaintCell::setDisplayList() {

		displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE);

		glBegin(GL_TRIANGLE_FAN);
		OpenGL::MathWrapper::glNormal(*normal);
		OpenGL::MathWrapper::glVertex(center);
		for (int j = Paint::PaintCell::CELL_VERTICES-1; j >= 0; j--) {
			OpenGL::MathWrapper::glVertex(*vertex[j]);
		}
		OpenGL::MathWrapper::glVertex(*vertex[Paint::PaintCell::CELL_VERTICES-1]);
		glEnd();

		glEndList();

	}

	PaintCell::~PaintCell() {
		optimize();
		glDeleteLists(displayList, 1);
	}

	void PaintCell::setBoundingDimensions() {
		setCorners(*vertex[0], *vertex[1]);
		for (int i = 2; i < CELL_VERTICES; i++) {
			expandToInclude(*vertex[i]);
		}
	}

	void PaintCell::calculateNormal() {
		normal = new Point();
		for (int i = 0; i < CELL_VERTICES; i++) {
			*normal += Geometry::triangleNormal(center, *(vertex[(i+1)%CELL_VERTICES]), *(vertex[i]));
		}
		(*normal).normalize();
	}

	void PaintCell::setVertex(int vert_index, Point _vertex) {
		delete vertex[vert_index];
		vertex[vert_index] = new Point(_vertex);
	}

	void PaintCell::setNormal(Math::Point _normal) {
		delete normal;
		normal = new Point(_normal);
	}

	Point PaintCell::calcVertexLocation(int vert_index, double cell_radius) {
		/*
		double angle = (double) vert_index * (PI/3.0);
		Point p = Point::point2D(cos(angle)*cell_radius, sin(angle)*cell_radius, PAINT_AXIS);
		return center+p;
		*/
		switch (vert_index) {
			case 0:
				return center+Point::point2D(cell_radius, 0.0, PAINT_AXIS);
			case 1:
				return center+Point::point2D(cell_radius*0.5, cell_radius*SQUARE_ROOT_THREE*0.5, PAINT_AXIS);
			case 2:
				return center+Point::point2D(-cell_radius*0.5, cell_radius*SQUARE_ROOT_THREE*0.5, PAINT_AXIS);
			case 3:
				return center+Point::point2D(-cell_radius, 0.0, PAINT_AXIS);
			case 4:
				return center+Point::point2D(-cell_radius*0.5, -cell_radius*SQUARE_ROOT_THREE*0.5, PAINT_AXIS);
			case 5:
				return center+Point::point2D(cell_radius*0.5, -cell_radius*SQUARE_ROOT_THREE*0.5, PAINT_AXIS);
			default:
				return center;
		}
	}

	void PaintCell::contractVertices(double amount) {
		for (int i = 0; i < CELL_VERTICES; i++) {
			*vertex[i] = (center*amount + *vertex[i]*(1.0-amount));
		}
	}

	void PaintCell::optimize() {

		if (vertex != NULL) {
			for (int i = 0; i < CELL_VERTICES; i++)
				delete vertex[i];
			delete[] vertex;
		}
		vertex = NULL;
		delete normal;
		normal = NULL;
	}

	void PaintCell::translateVertices(const Math::Point& translation) {
		center += translation;
		if (vertex) {
			for (int i = 0; i < CELL_VERTICES; i++)
				*vertex[i] += translation;
		}
	}

	std::ostream &operator << (std::ostream &stream, const PaintCell& cell) {

		stream << cell.center << ' ';
		for (int i = 0; i < Project::Paint::PaintCell::CELL_VERTICES; i++) {
			stream << *cell.vertex[i] << ' ';
		}
		stream << *cell.normal;
		return stream;

	}

	std::istream &operator >> (std::istream &stream, PaintCell& cell) {
		stream >> cell.center;
		//stream >> c;
		for (int i = 0; i < Project::Paint::PaintCell::CELL_VERTICES; i++) {
			Math::Point p;
			stream >> p;
			cell.setVertex(i, p);
		}
		Math::Point p;
		stream >> p;
		cell.setNormal(p);
		cell.setBoundingDimensions();
		return stream;
	}
}  // namespace Paint
}  // namespace Project
