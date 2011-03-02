#ifndef PROJECT_PAINT__PAINT_CELL_INFO_H
#define PROJECT_PAINT__PAINT_CELL_INFO_H

#include "math/Point.h"
#include "PaintCell.h"
#include "math/HexHeightMap.h"
#include "opengl/MathWrapper.h"
#include "math/BoundingBox3D.h"
#include <ostream>
#include <istream>

namespace Project {
namespace Paint {

class PaintCellInfo {

public:
	int uIndex;
	int vIndex;
	Math::Point normal;

	int layerIndex [PaintCell::CELL_VERTICES+1];

	PaintCellInfo(int u_index, int v_index);

	bool calcLayerIndices(const Math::HexHeightMap& height_map, int center_layer, double height_threshhold);
	void calcNormal(const Math::HexHeightMap& height_map);
	Math::BoundingBox3D getBoundingBox(const Math::HexHeightMap& height_map) const;
	void render(const Math::HexHeightMap& height_map) const;

	Math::Point vertexPoint(const Math::HexHeightMap& height_map, short vert_index) const;

	bool operator < (const PaintCellInfo& other) const { return normal < other.normal; }

};

std::ostream &operator << (std::ostream &stream, const PaintCellInfo& cell);
std::istream &operator >> (std::istream &stream, PaintCellInfo& cell);

}  // namespace Paint
}  // namespace Project

#endif
