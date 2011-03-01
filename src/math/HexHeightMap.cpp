#include "HexHeightMap.h"

namespace Project {
namespace Math {

	HexHeightMap::HexHeightMap(const HexGrid& hex_grid)
		: hexGrid(hex_grid) {

		int u_indices = hexGrid.numUIndices();
		int v_indices = hexGrid.numVIndices();

		hexHeight.resize(u_indices);
		vertexHeight.resize(u_indices*2);

		for (int u = 0; u < u_indices; u++) {

			hexHeight[u].resize(v_indices);
			vertexHeight[u*2].resize(v_indices);
			vertexHeight[u*2+1].resize(v_indices);

			for (int v = 0; v < v_indices; v++) {
				hexHeight[u][v] = NULL;
				vertexHeight[u*2][v] = NULL;
				vertexHeight[u*2+1][v] = NULL;
			}
		}
	}

	HexHeightMap::~HexHeightMap() {
		clear();
	}

	void HexHeightMap::addHexHeight(const HexGrid::HexIndex& hex_index, double height) {
		addHexHeight(hex_index.uIndex, hex_index.vIndex, height);
	}

	void HexHeightMap::addHexHeight(int u_index, int v_index, double height) {

		if (hexHeight[u_index][v_index] == NULL) {
			hexHeight[u_index][v_index] = new std::list<double>();

			struct HexGrid::HexIndex index;
			index.uIndex = u_index;
			index.vIndex = v_index;

			setHexIndices.push_back(index);
		}

		hexHeight[u_index][v_index]->push_back(height);
	}

	void HexHeightMap::addVertexHeight(const HexGrid::HexIndex& vert_index, double height) {
		addVertexHeight(vert_index.uIndex, vert_index.vIndex, height);
	}

	void HexHeightMap::addVertexHeight(int u_index, int v_index, double height) {

		if (vertexHeight[u_index][v_index] == NULL) {
			vertexHeight[u_index][v_index] = new std::list<double>();

			struct HexGrid::HexIndex index;
			index.uIndex = u_index;
			index.vIndex = v_index;

			setVertexIndices.push_back(index);
		}

		hexHeight[u_index][v_index]->push_back(height);

	}

	const std::list<double>* HexHeightMap::getHexHeights(const HexGrid::HexIndex& hex_index) const {
		return getHexHeights(hex_index.uIndex, hex_index.vIndex);
	}

	const std::list<double>* HexHeightMap::getHexHeights(int u_index, int v_index) const {
		return hexHeight[u_index][v_index];
	}

	const std::list<double>* HexHeightMap::getVertexHeights(const HexGrid::HexIndex& vert_index) const {
		return getVertexHeights(vert_index.uIndex, vert_index.vIndex);
	}

	const std::list<double>* HexHeightMap::getVertexHeights(int u_index, int v_index) const {
		return vertexHeight[u_index][v_index];
	}

	void HexHeightMap::clear() {

		for (unsigned int i = 0; i < setHexIndices.size(); i++)
			delete hexHeight[ setHexIndices[i].uIndex ] [ setHexIndices[i].vIndex ];

		setHexIndices.clear();

		for (unsigned int i = 0; i < setVertexIndices.size(); i++)
			delete vertexHeight[ setVertexIndices[i].uIndex ] [ setVertexIndices[i].vIndex ];

		setVertexIndices.clear();

	}

}  // namespace Math
}  // namespace Project
