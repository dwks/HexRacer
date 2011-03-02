#ifndef PROJECT_MATH__HEX_HEIGHT_MAP_H
#define PROJECT_MATH__HEX_HEIGHT_MAP_H

#include <list>
#include <vector>
#include "HexGrid.h"
#include "math/Point.h"

namespace Project {
namespace Math {

class HexHeightMap {
private:

	std::vector< std::vector< std::list<double>* > > hexHeight;
	std::vector< std::vector< std::list<double>* > > vertexHeight;

	std::vector<struct HexGrid::HexIndex> setHexIndices;
	std::vector<struct HexGrid::HexIndex> setVertexIndices;

	HexGrid hexGrid;

public:

	struct LayeredHexIndex {
		HexGrid::HexIndex hexIndex;
		int layerIndex;
	};

	struct HexIndexHeight {
		HexGrid::HexIndex index;
		double height;
		bool operator < (const HexIndexHeight& other) const {return height < other.height;}
	};

	HexHeightMap(const HexGrid& hex_grid = HexGrid());
	~HexHeightMap();

	void addHexHeight(const HexGrid::HexIndex& hex_index, double height);
	void addHexHeight(int u_index, int v_index, double height);

	void addVertexHeight(const HexGrid::HexIndex& vert_index, double height);
	void addVertexHeight(int u_index, int v_index, double height);

	const std::list<double>* getHexHeights(const HexGrid::HexIndex& hex_index) const;
	const std::list<double>* getHexHeights(int u_index, int v_index) const;

	const std::list<double>* getVertexHeights(const HexGrid::HexIndex& vert_index) const;
	const std::list<double>* getVertexHeights(int u_index, int v_index) const;

	const std::vector<struct HexGrid::HexIndex>& getSetHexIndices() const { return setHexIndices; }
	const std::vector<struct HexGrid::HexIndex>& getSetVertexIndices() const { return setVertexIndices; }

	void setHexGrid(const HexGrid& hex_grid);
	const HexGrid& getHexGrid() const { return hexGrid; }

	Math::Point hexToPoint(const LayeredHexIndex& hex_index, Math::Axis axis) const;
	Math::Point hexToPoint(int u_index, int v_index, int layer_index, Math::Axis axis) const;
	
	Math::Point vertexToPoint(const LayeredHexIndex& vert_index, Math::Axis axis) const;
	Math::Point vertexToPoint(int u_index, int v_index, int layer_index, Math::Axis axis) const;

	void clear();

	void operator = (const HexHeightMap& other);
	
};

std::ostream &operator << (std::ostream &stream, const HexHeightMap &object);
std::istream &operator >> (std::istream &stream, HexHeightMap &object);

}  // namespace Math
}  // namespace Project

#endif
