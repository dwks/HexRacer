#ifndef PROJECT_MATH__HEX_HEIGHT_MAP_H
#define PROJECT_MATH__HEX_HEIGHT_MAP_H

#include <list>
#include <vector>
#include "HexGrid.h"

namespace Project {
namespace Math {

class HexHeightMap {
private:

	std::vector< std::vector< std::list<double>* > > hexHeight;
	std::vector< std::vector< std::list<double>* > > vertexHeight;

	std::vector<struct HexGrid::HexIndex> setHexIndices;
	std::vector<struct HexGrid::HexIndex> setVertexIndices;

	const HexGrid& hexGrid;

public:

	HexHeightMap(const HexGrid& hex_grid);
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

	void clear();
	
};

}  // namespace Math
}  // namespace Project

#endif
