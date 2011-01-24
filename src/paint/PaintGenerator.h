#ifndef PROJECT_PAINT__PAINT_GENERATOR_H
#define PROJECT_PAINT__PAINT_GENERATOR_H

#include "PaintCell.h"
#include "math/Triangle3D.h"
#include "math/BSPTree3D.h"
#include "math/Vertex3D.h"
#include <vector>

namespace Project {
namespace Paint {

class PaintGenerator {
private:
	std::vector<PaintCell*> paintCells;
	Math::BSPTree3D* triangleTree;

	vector<double> heightsAtPoint(double u, double v);

public:
	PaintGenerator(std::vector<Math::Triangle3D> _triangles, double cell_radius = PAINT_CELL_RADIUS);
	std::vector<PaintCell*>& getPaintCells() { return paintCells; }
};

}  // namespace Paint
}  // namespace Project

#endif
