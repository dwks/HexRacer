#include "PaintGrid.h"

namespace Project {
namespace Paint {

	PaintGrid::PaintGrid(const Math::HexGrid& hex_grid) {

		cellArray.resize(hex_grid.numUIndices());
		for (unsigned int i = 0; i < cellArray.size(); i++) {
			cellArray[i].resize(hex_grid.numVIndices());
			for (unsigned int j = 0; j < cellArray[i].size(); j++) {
				cellArray[i][j] = NULL;
			}
		}
	}

	void PaintGrid::addPaintCell(const Math::HexGrid::HexIndex& index, PaintCell* cell) {
		addPaintCell(index.uIndex, index.vIndex, cell);
	}

	void PaintGrid::addPaintCell(int u_index, int v_index, PaintCell* cell) {
		PaintCell* old_cell = cellArray[u_index][v_index];
		cellArray[u_index][v_index] = cell;
		cell->nextCell = old_cell;
	}

	PaintCell* PaintGrid::getPaintCell(const Math::HexGrid::HexIndex& index) const {
		return cellArray[index.uIndex][index.vIndex];
	}

	PaintCell* PaintGrid::getPaintCell(int u_index, int v_index) const {
		return cellArray[u_index][v_index];
	}

}  // namespace Paint
}  // namespace Project
