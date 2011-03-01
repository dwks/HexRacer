#ifndef PROJECT_MATH__HEX_GRID_H
#define PROJECT_MATH__HEX_GRID_H

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

	HexGrid(double hex_radius, double min_u, double max_u, double min_v, double max_v);

	void setDimensions(double min_u, double max_u, double min_v, double max_v);
	void setHexRadius(double radius);

	double getMinU() const { return minU; }
	double getMinV() const { return minV; }
	double getHexRadius() const { return hexRadius; }
	double getHexHalfHeight() const { return hexHalfHeight; }

	int numUIndices() const { return uIndices; }
	int numVIndices() const { return vIndices; }

	double hexUPosition(int u_index, int v_index) const;
	double hexVPosition(int v_index) const;

	struct HexPosition hexPosition(const HexIndex& hex_index) const;
	struct HexPosition hexPosition(int u_index, int v_index) const;

	struct HexPosition vertexPosition(const HexIndex& vert_index) const;
	struct HexPosition vertexPosition(int u_index, int v_index) const;

	struct HexIndex vertexIndex(const HexIndex& hex_index, short vert_index) const;
	struct HexIndex vertexIndex(int u_index, int v_index, short vert_index) const;

	bool validHex(const HexIndex& hex_index) const;
	bool validVertex(const HexIndex& vert_index) const;
};

}  // namespace Math
}  // namespace Project

#endif
