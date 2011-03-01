#include "HexGrid.h"
#include "Values.h"

namespace Project {
namespace Math {

	HexGrid::HexGrid(double hex_radius, double min_u, double max_u, double min_v, double max_v) {

		hexRadius = abs(hex_radius);
		setDimensions(min_u, max_u, min_v, max_v);

		updateDimensions();
	}

	void HexGrid::updateDimensions() {
		
		uWidth = maxU-minU;
		vWidth = maxV-minV;

		hexUDivision = hexRadius*3.0;
		hexHalfHeight = sqrt(3.0)*hexRadius*0.5;
		oddRowOffset = -hexRadius*1.5;

		uIndices = (int) ceil( uWidth/hexUDivision );
		vIndices = (int) ceil( vWidth/hexHalfHeight );

	}

	void HexGrid::setDimensions(double min_u, double max_u, double min_v, double max_v) {
		minU = Math::minimum(min_u, max_u);
		maxU = Math::maximum(min_u, max_u);
		minV = Math::minimum(min_v, max_v);
		maxV = Math::maximum(min_v, max_v);
		updateDimensions();
	}

	double HexGrid::hexUPosition(int u_index, int v_index) const {
		return (
			minU+
			oddRowOffset*static_cast<double>(v_index % 2)+
			static_cast<double>(u_index)*hexUDivision
			);

	}

	double HexGrid::hexVPosition(int v_index) const {
		return minV+static_cast<double>(v_index)*hexHalfHeight;
	}

	struct HexGrid::HexPosition HexGrid::hexPosition(const HexGrid::HexIndex& hex_index) const {
		return hexPosition(hex_index.uIndex, hex_index.vIndex);
	}

	struct HexGrid::HexPosition HexGrid::hexPosition(int u_index, int v_index) const {
		struct HexPosition pos;
		pos.uPos = hexUPosition(u_index, v_index);
		pos.vPos = hexVPosition(v_index);
		return pos;
	}

	struct HexGrid::HexPosition HexGrid::vertexPosition(const HexGrid::HexIndex& vert_index) const {
		return vertexPosition(vert_index.uIndex, vert_index.vIndex);
	}

	struct HexGrid::HexPosition HexGrid::vertexPosition(int u_index, int v_index) const {
		struct HexPosition pos;
		if (u_index % 2)
			pos.uPos = hexUPosition(u_index/2, v_index)+hexRadius;
		else
			pos.uPos = hexUPosition(u_index/2, v_index)-hexRadius;
		pos.vPos = hexVPosition(v_index);
		return pos;
	}

	struct HexGrid::HexIndex HexGrid::vertexIndex(const HexGrid::HexIndex& hex_index, short vert_index) const {
		return vertexIndex(hex_index.uIndex, hex_index.vIndex, vert_index);
	}

	struct HexGrid::HexIndex HexGrid::vertexIndex(int u_index, int v_index, short vert_index) const {

		struct HexIndex vert;
		short odd_row = static_cast<short>(v_index % 2);
		short even_row = static_cast<short>(odd_row == 0);
			
		switch (vert_index) {
			case 0:
				vert.uIndex = (u_index*2)+1;
				vert.vIndex = v_index;
				break;
			case 1:
				vert.uIndex = (u_index+even_row)*2;
				vert.vIndex = v_index+1;
				break;
			case 2:
				vert.uIndex = (u_index-odd_row)*2+1;
				vert.vIndex = v_index+1;
				break;
			case 3:
				vert.uIndex = (u_index*2);
				vert.vIndex = v_index;
				break;
			case 4:
				vert.uIndex = (u_index-odd_row)*2+1;
				vert.vIndex = v_index-1;
				break;
			case 5:
				vert.uIndex = (u_index+even_row)*2;	
				vert.vIndex = v_index-1;
				break;
			default:
				break;

		}

		return vert;

	}

	bool HexGrid::validHex(const HexIndex& hex_index) const {
		return (hex_index.uIndex >= 0 && hex_index.uIndex < uIndices 
			&& hex_index.vIndex >= 0 && hex_index.vIndex < vIndices);
	}

	bool HexGrid::validVertex(const HexIndex& vert_index) const {
		struct HexIndex hex(vert_index);
		hex.uIndex /= 2;
		return validHex(hex);
	}

}  // namespace Math
}  // namespace Project
