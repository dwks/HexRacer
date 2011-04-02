#include "HexGrid.h"
#include "Values.h"
#include <cmath>

namespace Project {
namespace Math {

	HexGrid::HexGrid(double hex_radius, double min_u, double max_u, double min_v, double max_v) {

		hexRadius = std::abs(hex_radius);
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

	void HexGrid::setHexRadius(double radius) {
		hexRadius = fabs(radius);
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

	struct HexGrid::HexIndex HexGrid::vertexIndex(const HexGrid::HexIndex& hex_index, short vert_index) {
		return vertexIndex(hex_index.uIndex, hex_index.vIndex, vert_index);
	}

	struct HexGrid::HexIndex HexGrid::vertexIndex(int u_index, int v_index, short vert_index) {

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
            default:
				vert.uIndex = (u_index+even_row)*2;	
				vert.vIndex = v_index-1;
				break;

		}

		return vert;

	}

	bool HexGrid::validHex(const HexIndex& hex_index) const {
		return (hex_index.uIndex >= 0 && hex_index.uIndex < uIndices 
			&& hex_index.vIndex >= 0 && hex_index.vIndex < vIndices);
	}

	bool HexGrid::validHex(int u_index, int v_index) const {
		return (u_index >= 0 && u_index < uIndices &&
			v_index >= 0 && v_index < vIndices);
	}

	bool HexGrid::validVertex(const HexIndex& vert_index) const {
		return validVertex(vert_index.uIndex, vert_index.vIndex);
	}

	bool HexGrid::validVertex(int u_index, int v_index) const {

		if (u_index > 0)
			u_index /= 2;

		return (u_index >= 0 && u_index < uIndices &&
			v_index >= 0 && v_index < vIndices);
	}





	struct HexGrid::HexIndexRange HexGrid::hexIndexRange(const BoundingObject2D& obj) const {
		HexIndexRange range;
		range.minUIndex = static_cast<int>( std::ceil( (obj.minU()-hexRadius-minU+oddRowOffset)/hexUDivision ) );
		range.maxUIndex = static_cast<int>( (obj.maxU()+hexRadius-minU)/hexUDivision );
		range.minVIndex = static_cast<int>( std::ceil((obj.minV()-hexHalfHeight-minV)/hexHalfHeight ) );
		range.maxVIndex = static_cast<int>( (obj.maxV()+hexHalfHeight-minV)/hexHalfHeight );
		return range;
	}

	bool HexGrid::validRange(HexGrid::HexIndexRange& range) const {
		if (range.maxUIndex < 0 || range.minUIndex >= uIndices ||
			range.maxVIndex < 0 || range.minVIndex >= vIndices)
			return false;

		range.minUIndex = Math::maximum(range.minUIndex, 0);
		range.maxUIndex = Math::minimum(range.maxUIndex, uIndices-1);
		range.minVIndex = Math::maximum(range.minVIndex, 0);
		range.maxVIndex = Math::minimum(range.maxVIndex, vIndices-1);

		return true;
	}

	std::ostream &operator << (std::ostream &stream, const HexGrid::HexIndex &index) {
		stream << index.uIndex << ' ' << index.vIndex;
		return stream;
	}

	std::istream &operator >> (std::istream &stream, HexGrid::HexIndex &index) {
		stream >> index.uIndex >> index.vIndex;
		return stream;
	}


	std::ostream &operator << (std::ostream &stream, const HexGrid &object) {
		stream << object.getHexRadius() << ' '
			<< object.getMinU() << ' ' << object.getMaxU() << ' '
			<< object.getMinV() << ' ' << object.getMaxV();
		return stream;
	}

	std::istream &operator >> (std::istream &stream, HexGrid &object) {
		double hex_radius;
		double min_u;
		double max_u;
		double min_v;
		double max_v;
		stream >> hex_radius >> min_u >> max_u >> min_v >> max_v;
		object.setHexRadius(hex_radius);
		object.setDimensions(min_u, max_u, min_v, max_v);
		return stream;
	}
}  // namespace Math
}  // namespace Project
