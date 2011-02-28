#ifndef PROJECT_PAINT__PAINT_CELL_H
#define PROJECT_PAINT__PAINT_CELL_H

#include "opengl/OpenGL.h"
#include "math/BoundingBox3D.h"
#include "config.h"

namespace Project {
namespace Paint {

class PaintCell 
	: public Math::BoundingBox3D {
public:

	static const int CELL_VERTICES = 6;
	static const Math::Axis PAINT_AXIS = Math::Y_AXIS;

	PaintCell(Math::Point _center);
	PaintCell(Math::Point _center, Math::Point* vertices, Math::Point _normal, int _index);
	~PaintCell();

	Math::Point** vertex;
	Math::Point* normal;
	Math::Point center;
	int playerColor;
	int index;
	GLuint displayList;

	int getOppositeVertex(int vert_index);
	void setBoundingDimensions();
	void calculateNormal();
	void contractVertices(double amount);
	void setVertex(int vert_index, Math::Point _vertex);
	void setNormal(Math::Point _normal);
	void setDisplayList();
	void optimize();
	void translateVertices(const Math::Point& translation);
	Math::Point calcVertexLocation(int vert_index, double cell_radius = PAINT_CELL_RADIUS);

};

std::ostream &operator << (std::ostream &stream, const PaintCell& cell);
std::istream &operator >> (std::istream &stream, PaintCell& cell);

}  // namespace Paint
}  // namespace Project

#endif
