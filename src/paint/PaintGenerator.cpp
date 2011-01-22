#include "PaintGenerator.h"
#include "math/BoundingBox3D.h"
#include "math/BoundingBox2D.h"
#include "math/Vertex2D.h"
#include "math/Geometry.h"
#include "log/Logger.h"
#include <math.h>
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Paint {

	PaintGenerator::PaintGenerator(std::vector<Triangle3D> _triangles) {

		if (_triangles.size() <= 0)
			return;

		LOG(PAINT, "Generating Paint Cells... ");

		vector<ObjectSpatial*> triangles;

		BoundingBox3D paint_bound(_triangles[0]);
		triangles.push_back(new Triangle3D(_triangles[0]));
		for (unsigned int i = 1; i < _triangles.size(); i++) {
			paint_bound.expandToInclude(_triangles[i]);
			triangles.push_back(new Triangle3D(_triangles[i]));
		}

		triangleTree = new BSPTree3D(paint_bound);
		triangleTree->add(triangles);

		double cell_u_division = PAINT_CELL_RADIUS*3.0;
		double cell_half_height = sqrt(3.0)*PAINT_CELL_RADIUS*0.5;
		double odd_row_offset = PAINT_CELL_RADIUS*1.5;

		BoundingBox2D b2D(paint_bound, PaintCell::PAINT_AXIS);

		double start_u = b2D.minU();
		double start_v = b2D.minV();
		double end_u = b2D.maxU();
		double end_v = b2D.maxV();

		//Generate Cells
		bool even_row = true;
		for (double v = start_v; v <= end_v; v += cell_half_height) {

			double offset_u = 0.0;
			if (!even_row)
				offset_u =-odd_row_offset;

			for (double u = start_u+offset_u; u <= end_u; u += cell_u_division) {


				vector<double> heights = heightsAtPoint(u, v);
				for (unsigned int i = 0; i < heights.size(); i++) {

					Point cell_point = Point::point2D(u, v, PaintCell::PAINT_AXIS);
					cell_point.setCoord(heights[i], PaintCell::PAINT_AXIS);
					paintCells.push_back(new PaintCell(cell_point));

				}

			}

			even_row = !even_row;
		}

		//Generate cell vertices
		for (unsigned int i = 0; i < paintCells.size(); i++) {
			for (int v = 0; v < PaintCell::CELL_VERTICES; v++) {

				Point wanted_point = paintCells[i]->calcVertexLocation(v);

				//Find the heights at the wanted point
				vector<double> heights = heightsAtPoint(
					wanted_point.getU(PaintCell::PAINT_AXIS),
					wanted_point.getV(PaintCell::PAINT_AXIS));

				if (heights.size() > 0) {

					bool height_found = false;
					double wanted_height = wanted_point.getCoord(PaintCell::PAINT_AXIS);
					double min_height_difference = PAINT_CELL_VERTEX_HEIGHT_THRESHHOLD;
					double closest_height = wanted_height;

					//Get the closest height value to the wanted height value
					for (unsigned int h = 0; h < heights.size(); h++) {
						double height_difference = fabs(heights[h]-wanted_height);
						if (height_difference < min_height_difference) {
							min_height_difference = height_difference;
							closest_height = heights[h];
							height_found = true;
						}
					}

					if (height_found) {
						wanted_point.setCoord(closest_height, PaintCell::PAINT_AXIS);
						paintCells[i]->setVertex(v, wanted_point);
					}
				}

			}
		}

		//Delete all paint cells with a missing vertex
		for (int i = paintCells.size()-1; i >= 0; i--) {
			bool delete_cell = false;
			for (int v = 0; v < PaintCell::CELL_VERTICES && !delete_cell; v++) {
				delete_cell = (delete_cell || !paintCells[i]->vertexSet[v]);
			}
			if (delete_cell) {
				delete(paintCells[i]);
				paintCells[i] = paintCells[paintCells.size()-1];
				paintCells.resize(paintCells.size()-1);
			}
		}

		Point translation;
		translation.setCoord(PAINT_CELL_LIFT_AMOUNT, PaintCell::PAINT_AXIS);

		//Finalize the paint cells
		for (unsigned int i = 0; i < paintCells.size(); i++) {
			//paintCells[i]->fillEmptyVertices();
			paintCells[i]->setBoundingDimensions();
			paintCells[i]->calculateNormal();
			paintCells[i]->center += translation;
			for (int v = 0; v < PaintCell::CELL_VERTICES; v++) {
				paintCells[i]->vertex[v] += translation;
			}
			paintCells[i]->contractVertices(PAINT_CELL_CONTRACTION);
		}


		LOG(PAINT, "Generated " << paintCells.size() << " paint cells! ");
	}

	vector<double> PaintGenerator::heightsAtPoint(double u, double v) {

		vector<double> heights;

		Point p = Point::point2D(u, v, PaintCell::PAINT_AXIS);
		Point d;
		d.setCoord(1.0, PaintCell::PAINT_AXIS);
		Ray r(p, d);

		//Find intersecting triangles
		vector<ObjectSpatial*> tris =
			triangleTree->query(Vertex2D(u, v, PaintCell::PAINT_AXIS), SpatialContainer::INTERSECT);

		for (unsigned int i = 0; i < tris.size(); i++) {

			Triangle3D* tri = ((Triangle3D*)tris[i]);
			RayIntersection intersect = Geometry::rayPlaneIntersection(r, tri->getVertex(0), tri->getNormal());
			if (intersect.intersects) {
				heights.push_back(r.atT(intersect.t).getCoord(PaintCell::PAINT_AXIS));
			}
		}

		return heights;
	}

}  // namespace Paint
}  // namespace Project
