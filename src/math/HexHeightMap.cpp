#include "HexHeightMap.h"
#include "misc/StdListFunctions.h"
#include <istream>
#include <ostream>

namespace Project {
namespace Math {

	HexHeightMap::HexHeightMap(const HexGrid& hex_grid)
		: hexGrid(hex_grid) {

		setHexGrid(hexGrid);
	}

	HexHeightMap::~HexHeightMap() {
		clear();
	}

	void HexHeightMap::setHexGrid(const HexGrid& hex_grid) {

		clear();

		hexGrid = hex_grid;

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

		vertexHeight[u_index][v_index]->push_back(height);

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

	Math::Point HexHeightMap::hexToPoint(const HexHeightMap::LayeredHexIndex& hex_index, Math::Axis axis) const {
		return hexToPoint(hex_index.hexIndex.uIndex, hex_index.hexIndex.vIndex, hex_index.layerIndex, axis);
	}

	Math::Point HexHeightMap::hexToPoint(int u_index, int v_index, int layer_index, Math::Axis axis) const {
		Math::Point p = Math::Point::point2D(
			hexGrid.hexUPosition(u_index, v_index),
			hexGrid.hexVPosition(v_index),
			axis);

		p.setCoord(Misc::listElement(*(hexHeight[u_index][v_index]), layer_index), axis);

		return p;
	}
	Math::Point HexHeightMap::vertexToPoint(const HexHeightMap::LayeredHexIndex& vert_index, Math::Axis axis) const {
		return vertexToPoint(vert_index.hexIndex.uIndex, vert_index.hexIndex.vIndex, vert_index.layerIndex, axis);
	}

	Math::Point HexHeightMap::vertexToPoint(int u_index, int v_index, int layer_index, Math::Axis axis) const {

		HexGrid::HexPosition position = hexGrid.vertexPosition(u_index, v_index);
		Math::Point p = Math::Point::point2D(position.uPos, position.vPos, axis);
		p.setCoord(Misc::listElement(*(vertexHeight[u_index][v_index]), layer_index), axis);

		return p;
	}

	void HexHeightMap::clear() {

		for (unsigned int i = 0; i < setHexIndices.size(); i++) {
			delete hexHeight[ setHexIndices[i].uIndex ] [ setHexIndices[i].vIndex ];
			hexHeight[ setHexIndices[i].uIndex ] [ setHexIndices[i].vIndex ] = NULL;
		}

		setHexIndices.clear();

		for (unsigned int i = 0; i < setVertexIndices.size(); i++) {
			delete vertexHeight[ setVertexIndices[i].uIndex ] [ setVertexIndices[i].vIndex ];
			vertexHeight[ setVertexIndices[i].uIndex ] [ setVertexIndices[i].vIndex ] = NULL;
		}

		setVertexIndices.clear();

	}


	void HexHeightMap::operator = (const HexHeightMap& other) {
		setHexGrid(other.getHexGrid());

		const std::vector<struct HexGrid::HexIndex>& hex_indices = other.getSetHexIndices();
		for (unsigned int i = 0; i < hex_indices.size(); i++) {
			const std::list<double>* heights = other.getHexHeights(hex_indices[i]);
			for (std::list<double>::const_iterator it = heights->begin(); it != heights->end(); ++it) {
				addHexHeight(hex_indices[i], *it);
			}
		}

		const std::vector<struct HexGrid::HexIndex>& vert_indices = other.getSetVertexIndices();
		for (unsigned int i = 0; i < vert_indices.size(); i++) {
			const std::list<double>* heights = other.getVertexHeights(vert_indices[i]);
			for (std::list<double>::const_iterator it = heights->begin(); it != heights->end(); ++it) {
				addVertexHeight(vert_indices[i], *it);
			}
		}
	}

	std::ostream &operator << (std::ostream &stream, const HexHeightMap &object) {

		const std::vector<HexGrid::HexIndex>& set_hex_indices = object.getSetHexIndices();
		stream << set_hex_indices.size() << '\n';
		for (unsigned int i = 0; i < set_hex_indices.size(); i++) {

			const std::list<double>* list = object.getHexHeights(set_hex_indices[i]);
			stream << set_hex_indices[i] << ' ' << list->size() << ' ';
			for (std::list<double>::const_iterator it = list->begin(); it != list->end(); ++it) {
				stream << *it << ' ';
			}
			stream << '\n';
		}

		const std::vector<HexGrid::HexIndex>& set_vert_indices = object.getSetVertexIndices();
		stream << set_vert_indices.size() << '\n';
		for (unsigned int i = 0; i < set_vert_indices.size(); i++) {
			const std::list<double>* list = object.getVertexHeights(set_vert_indices[i]);
			stream << set_vert_indices[i] << ' ' << list->size() << ' ';
			for (std::list<double>::const_iterator it = list->begin(); it != list->end(); ++it) {
				stream << *it << ' ';
			}
			stream << '\n';
		}

		return stream;

	}

	std::istream &operator >> (std::istream &stream, HexHeightMap &object) {

		//Load hex hights
		int indices;
		stream >> indices;
		while (indices > 0) {
			HexGrid::HexIndex index;
			int heights;
			stream >> index;
			stream >> heights;
			while (heights > 0) {
				double height;
				stream >> height;
				object.addHexHeight(index, height);
				heights--;
			}
			indices--;
		}

		//Load vertex hights
		stream >> indices;
		while (indices > 0) {
			HexGrid::HexIndex index;
			int heights;
			stream >> index;
			stream >> heights;
			while (heights > 0) {
				double height;
				stream >> height;
				object.addVertexHeight(index, height);
				heights--;
			}
			indices--;
		}

		return stream;
	}

}  // namespace Math
}  // namespace Project
