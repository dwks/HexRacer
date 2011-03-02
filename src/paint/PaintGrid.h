#ifndef PROJECT_PAINT__PAINT_GRID_H
#define PROJECT_PAINT__PAINT_GRID_H

#include "PaintCell.h"
#include "math/HexGrid.h"
#include <vector>
#include <list>

namespace Project {
namespace Paint {

class PaintGrid {
private:
	std::vector< std::vector<PaintCell*> > cellArray;
public:
	PaintGrid(const Math::HexGrid& hex_grid = Math::HexGrid());
	void addPaintCell(const Math::HexGrid::HexIndex& index, PaintCell* cell);
	void addPaintCell(int u_index, int v_index, PaintCell* cell);
	PaintCell* getPaintCell(const Math::HexGrid::HexIndex& index) const;
	PaintCell* getPaintCell(int u_index, int v_index) const;
};

}  // namespace Paint
}  // namespace Project

#endif
