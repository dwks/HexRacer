#ifndef PROJECT_MATH__HEX_GRID_H
#define PROJECT_MATH__HEX_GRID_H

#include <istream>
#include <ostream>
#include "BoundingObject2D.h"

namespace Project {
namespace Math {

class HexGrid {
private:

	double minU;
	double maxU;
	double minV;
	double maxV;
	
	double uWidth;
	double vWidth;

	double hexRadius;
	int uIndices;
	int vIndices;

	double hexUDivision;
	double hexHalfHeight;
	double oddRowOffset;

	void updateDimensions();

public:

	struct HexIndex {
		int uIndex;
		int vIndex;
	};

	struct HexPosition {
		double uPos;
		double vPos;
	};

	struct HexIndexRange {
		int minUIndex;
		int maxUIndex;
		int minVIndex;
		int maxVIndex;
	};

	HexGrid(double hex_radius = 0.1, double min_u = 0.0, double max_u = 0.0, double min_v = 0.0, double max_v = 0.0);

	void setDimensions(double min_u, double max_u, double min_v, double max_v);
	void setHexRadius(double radius);

	double getMinU() const { return minU; }
	double getMinV() const { return minV; }
	double getMaxU() const { return maxU; }
	double getMaxV() const { return maxV; }
	double getHexRadius() const { return hexRadius; }
	double getHexHalfHeight() const { return hexHalfHeight; }

	int numUIndices() const { return uIndices; }
	int numVertexUIndices() const { return uIndices*2; }
	int numVIndices() const { return vIndices; }

	double hexUPosition(int u_index, int v_index) const;
	double hexVPosition(int v_index) const;

	struct HexPosition hexPosition(const HexIndex& hex_index) const;
	struct HexPosition hexPosition(int u_index, int v_index) const;

	struct HexPosition vertexPosition(const HexIndex& vert_index) const;
	struct HexPosition vertexPosition(int u_index, int v_index) const;

	struct HexIndexRange queryIndexRange(const BoundingObject2D& obj) const;

	bool validRange(HexIndexRange& range) const;

	static struct HexIndex vertexIndex(const HexIndex& hex_index, short vert_index);
	static struct HexIndex vertexIndex(int u_index, int v_index, short vert_index);

	bool validHex(const HexIndex& hex_index) const;
	bool validHex(int u_index, int v_index) const;
	bool validVertex(const HexIndex& vert_index) const;
	bool validVertex(int u_index, int v_index) const;
};

std::ostream &operator << (std::ostream &stream, const HexGrid::HexIndex &index);
std::istream &operator >> (std::istream &stream, HexGrid::HexIndex &index);

std::ostream &operator << (std::ostream &stream, const HexGrid &object);
std::istream &operator >> (std::istream &stream, HexGrid &object);

}  // namespace Math
}  // namespace Project

#endif
