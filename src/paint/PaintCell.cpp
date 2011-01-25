#include "PaintCell.h"
#include "math/Geometry.h"
#include "math/Values.h"
using namespace Project;
using namespace Math;

#ifndef SQUARE_ROOT_THREE
#define SQUARE_ROOT_THREE 1.7320508075688772935274463415058723669428052538103806280558
#endif

namespace Project {
namespace Paint {

	PaintCell::PaintCell(Point _center) {
		playerColor = -1;
		center = _center;
		for (int i = 0; i < CELL_VERTICES; i++) {
			vertexSet[i] = false;
		}
	}

	PaintCell::PaintCell(Math::Point _center, Math::Point* vertices, Math::Point _normal, int _index) {
		playerColor = -1;
		center = _center;
		for (int i = 0; i < CELL_VERTICES; i++) {
			vertex[i] = vertices[i];
			vertexSet[i] = true;
		}
		normal = _normal;
		index = _index;
		setBoundingDimensions();
	}

	void PaintCell::setBoundingDimensions() {
		setCorners(vertex[0], vertex[1]);
		for (int i = 2; i < CELL_VERTICES; i++) {
			expandToInclude(vertex[i]);
		}
	}

	void PaintCell::calculateNormal() {
		normal = Point();
		for (int i = 0; i < CELL_VERTICES; i++) {
			normal += Geometry::triangleNormal(center, vertex[i], vertex[(i+1)%CELL_VERTICES]);
		}
		normal /= static_cast<double>(CELL_VERTICES);
	}

	int PaintCell::getOppositeVertex(int vert_index) {
		return (vert_index+(CELL_VERTICES/2)) % CELL_VERTICES;
	}

	void PaintCell::fillEmptyVertices() {
		for (int i = 0; i < CELL_VERTICES; i++) {
			if (!vertexSet[i]) {
				int opposite = getOppositeVertex(i);
				if (!vertexSet[opposite]) {
					setVertex(i, calcVertexLocation(i));
					setVertex(opposite, calcVertexLocation(opposite));
				}
				else {
					Point p = calcVertexLocation(i);
					double height_delta =
						center.getCoord(PAINT_AXIS)-vertex[opposite].getCoord(PAINT_AXIS);
					p.setCoord(center.getCoord(PAINT_AXIS) + height_delta, PAINT_AXIS);
					setVertex(i, p);
				}
			}
		}
	}

	void PaintCell::setVertex(int vert_index, Point _vertex) {
		vertex[vert_index] = _vertex;
		vertexSet[vert_index] = true;
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
			vertex[i] = (center*amount + vertex[i]*(1.0-amount));
		}
	}

}  // namespace Paint
}  // namespace Project
