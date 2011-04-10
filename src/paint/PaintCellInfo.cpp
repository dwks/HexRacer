#include "PaintCellInfo.h"
#include "misc/StdListFunctions.h"
#include <math.h>
#include "math/Geometry.h"
#include <istream>
#include <ostream>

namespace Project {
namespace Paint {

	PaintCellInfo::PaintCellInfo(int u_index, int v_index) {
		uIndex = u_index;
		vIndex = v_index;
	}

	bool PaintCellInfo::calcLayerIndices(const Math::HexHeightMap& height_map, int center_layer, double height_threshhold) {
		layerIndex[0] = center_layer;

		if (!height_map.getHexGrid().validHex(uIndex, vIndex))
			return false;

		const std::list<double>* heights = height_map.getHexHeights(uIndex, vIndex);
		if (!heights || static_cast<int>(heights->size()) <= center_layer)
			return false;

		double center_height = Misc::listElement(*heights, center_layer);

		for (int i = 0; i < PaintCell::CELL_VERTICES; i++) {

			Math::HexGrid::HexIndex vert_index = height_map.getHexGrid().vertexIndex(uIndex, vIndex, i);
			if (!height_map.getHexGrid().validVertex(vert_index))
				return false;

			const std::list<double>* heights = height_map.getVertexHeights(vert_index);

			if (!heights)
				return false;
			
			bool layer_found = false;
			int best_layer = 0;
			double min_height_diff = fabs(height_threshhold);

			int layer = 0;
			std::list<double>::const_iterator it;
			for (it = heights->begin(); it != heights->end(); it++) {
				double layer_height = *it;
				double height_diff = fabs(layer_height-center_height);

				if (height_diff < min_height_diff) {
					min_height_diff = height_diff;
					layer_found = true;
					best_layer = layer;
				}

				layer++;
			}

			if (!layer_found)
				return false;

			layerIndex[i+1] = best_layer;
		}

		return true;
	}

	void PaintCellInfo::calcNormal(const Math::HexHeightMap& height_map) {

		normal = Math::Point();

		Math::Point center_pos = centerPoint(height_map);
		for (int i = 0; i < PaintCell::CELL_VERTICES; i++) {

			Math::Point vertex_point = vertexPoint(height_map, i);
			
			Math::HexGrid::HexIndex next_vert_index = Math::HexGrid::vertexIndex(uIndex, vIndex, (i+1) % PaintCell::CELL_VERTICES);
			normal += Math::Geometry::triangleNormal(center_pos,
				vertexPoint(height_map, (i+1) % PaintCell::CELL_VERTICES),
				vertex_point);
		}

		normal.normalize();

	}

	Math::Point PaintCellInfo::centerPoint(const Math::HexHeightMap& height_map) const {
		return height_map.hexToPoint(uIndex, vIndex, layerIndex[0], PaintCell::PAINT_AXIS);
	}
	Math::Point PaintCellInfo::vertexPoint(const Math::HexHeightMap& height_map, short vert_index) const {
		Math::HexGrid::HexIndex vert_hex_index = Math::HexGrid::vertexIndex(uIndex, vIndex, vert_index);
		return height_map.vertexToPoint(vert_hex_index.uIndex, vert_hex_index.vIndex, layerIndex[vert_index+1], PaintCell::PAINT_AXIS);
	}

	Math::BoundingBox3D PaintCellInfo::getBoundingBox(const Math::HexHeightMap& height_map) const {

		Math::BoundingBox3D bounding_box;

		Math::Point center_pos = centerPoint(height_map);
		center_pos.setCoord(center_pos.getCoord(PaintCell::PAINT_AXIS)+PAINT_CELL_LIFT_AMOUNT, PaintCell::PAINT_AXIS);

		bounding_box.setCorners(center_pos, center_pos);

		for (short i = 0; i < PaintCell::CELL_VERTICES; i++) {
			Math::Point p = vertexPoint(height_map, i);
			p.setCoord(p.getCoord(PaintCell::PAINT_AXIS)+PAINT_CELL_LIFT_AMOUNT, PaintCell::PAINT_AXIS);
			bounding_box.expandToInclude(p);
		}

		return bounding_box;
	}

	void PaintCellInfo::render(const Math::HexHeightMap& height_map) const {

		Math::Point center_pos = centerPoint(height_map);
		center_pos.setCoord(center_pos.getCoord(PaintCell::PAINT_AXIS)+PAINT_CELL_LIFT_AMOUNT, PaintCell::PAINT_AXIS);

		Math::Point vertices [PaintCell::CELL_VERTICES];
		for (short i = 0; i < PaintCell::CELL_VERTICES; i++) {
			vertices[i] = vertexPoint(height_map, i);
			vertices[i].setCoord(vertices[i].getCoord(PaintCell::PAINT_AXIS)+PAINT_CELL_LIFT_AMOUNT, PaintCell::PAINT_AXIS);
		}

		OpenGL::MathWrapper::glNormal(normal);
		glBegin(GL_TRIANGLE_FAN);
		OpenGL::MathWrapper::glVertex(center_pos);
		for (short i = PaintCell::CELL_VERTICES-1; i >= 0; i--) {
			OpenGL::MathWrapper::glVertex(
				vertices[i]*(1.0-PAINT_CELL_CONTRACTION) + center_pos*PAINT_CELL_CONTRACTION
			);
		}
		OpenGL::MathWrapper::glVertex(
			vertices[PaintCell::CELL_VERTICES-1]*(1.0-PAINT_CELL_CONTRACTION) + center_pos*PAINT_CELL_CONTRACTION
			);
		glEnd();
	}

	std::ostream &operator << (std::ostream &stream, const PaintCellInfo& cell) {
		stream << cell.uIndex << ' ' << cell.vIndex << ' ';
		for (int i = 0; i <= PaintCell::CELL_VERTICES; i++) {
			stream << cell.layerIndex[i] << ' ';
		}
		return stream;
	}

	std::istream &operator >> (std::istream &stream, PaintCellInfo& cell) {
		stream >> cell.uIndex >> cell.vIndex;
		for (int i = 0; i <= PaintCell::CELL_VERTICES; i++) {
			stream >> cell.layerIndex[i];
		}
		return stream;
	}

}  // namespace Paint
}  // namespace Project
