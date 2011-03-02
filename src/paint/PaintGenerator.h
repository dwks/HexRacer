#ifndef PROJECT_PAINT__PAINT_GENERATOR_H
#define PROJECT_PAINT__PAINT_GENERATOR_H

#include "PaintCell.h"
#include "math/Triangle3D.h"
#include "math/BSPTree3D.h"
#include "math/Vertex3D.h"
#include "math/BoundingBox3D.h"
#include "math/HexGrid.h"
#include "math/HexHeightMap.h"
#include "PaintCellInfo.h"
#include <vector>

namespace Project {
namespace Paint {

class PaintGenerator {
private:
	std::vector<PaintCell*> paintCells;
	std::vector<PaintCellInfo> cellInfo;
	Math::BSPTree3D* triangleTree;
	Math::BoundingBox3D paintBound;

	void heightsAtPoint(double u, double v, vector<double>& vec);

	Math::HexHeightMap heightMap;

public:

	PaintGenerator();
	void generateHeightmap(const std::vector<Math::Triangle3D>& _triangles, const Math::HexGrid& hex_grid);
	void generateCells();
	//std::vector<PaintCell*>& getPaintCells() { return paintCells; }
	std::vector<PaintCellInfo>& getCellInfo() { return cellInfo; }

	Math::HexHeightMap& getHeightMap() { return heightMap; }
};

}  // namespace Paint
}  // namespace Project

#endif
