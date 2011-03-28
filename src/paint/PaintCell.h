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

	PaintCell(const Math::Point& _position);
	~PaintCell();

	int playerColor;
	GLuint displayList;
	PaintCell* nextCell;
	Math::Point position;

};

}  // namespace Paint
}  // namespace Project

#endif
