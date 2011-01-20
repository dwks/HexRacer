#ifndef PROJECT_PAINT__PAINT_VERTEX_H
#define PROJECT_PAINT__PAINT_VERTEX_H

#include "math/Vertex3D.h"

namespace Project {
namespace Paint {

class PaintVertex 
	: public Math::Vertex3D {
public:

	int numUsers;

	PaintVertex(double x, double y, double z)
		: Vertex3D(x, y, z) {
		numUsers = 0;
	}
};

}  // namespace Paint
}  // namespace Project

#endif
