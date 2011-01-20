#ifndef PROJECT_PAINT__PAINT_CELL_H
#define PROJECT_PAINT__PAINT_CELL_H

#include "math/BoundingBox3D.h"

namespace Project {
namespace Paint {

#define PAINT_CELL_RADIUS 1.0
#define PAINT_CELL_VERTEX_HEIGHT_THRESHHOLD 1.0
#define PAINT_CELL_LIFT_AMOUNT 0.1
#define PAINT_CELL_CONTRACTION 0.1

class PaintCell 
	: public Math::BoundingBox3D {
public:

	static const int CELL_VERTICES = 6;
	static const Math::Axis PAINT_AXIS = Math::Y_AXIS;

	PaintCell(Math::Point _center);

	bool vertexSet [CELL_VERTICES];
	Math::Point vertex [CELL_VERTICES];
	Math::Point normal;
	Math::Point center;
	int playerColor;

	int getOppositeVertex(int vert_index);
	void fillEmptyVertices();
	void setBoundingDimensions();
	void calculateNormal();
	void contractVertices(double amount);
	void setVertex(int vert_index, Math::Point _vertex);
	Math::Point calcVertexLocation(int vert_index);

};

}  // namespace Paint
}  // namespace Project

#endif
