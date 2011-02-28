#include "PaintGenerator.h"

#include "math/BoundingBox2D.h"
#include "math/Vertex2D.h"
#include "math/Geometry.h"
#include <math.h>
#ifdef QT_GUI_LIB
#include <QtGui>
#endif
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Paint {

	PaintGenerator::PaintGenerator(std::vector<Triangle3D> _triangles, double cell_radius) {

		if (_triangles.size() <= 0)
			return;

		//LOG(PAINT, "Generating Paint Cells...");

		vector<ObjectSpatial*> triangles;

		paintBound.setToObject(_triangles[0]);
		triangles.push_back(new Triangle3D(_triangles[0]));
		for (unsigned int i = 1; i < _triangles.size(); i++) {
			paintBound.expandToInclude(_triangles[i]);
			triangles.push_back(new Triangle3D(_triangles[i]));
		}

		triangleTree = new BSPTree3D(paintBound);
		triangleTree->add(triangles);

		double cell_u_division = cell_radius*3.0;
		double cell_half_height = sqrt(3.0)*cell_radius*0.5;
		double odd_row_offset = -cell_radius*1.5;

		BoundingBox2D b2D(paintBound, PaintCell::PAINT_AXIS);

		double start_u = b2D.minU()-cell_radius;
		double end_u = b2D.maxU()+cell_radius;
		double start_v = b2D.minV()-cell_half_height;
		double end_v = b2D.maxV()+cell_half_height;

		int u_steps = (int) ceil((end_u-start_u)/cell_u_division);
		int v_steps = (int) ceil((end_v-start_v)/cell_half_height);

		vector< vector< vector<PaintCell*> > > cell_array;
		vector< vector< vector<double> > > border_heights;

		cell_array.resize(u_steps);
		border_heights.resize(u_steps*2);
		for (int i = 0; i < u_steps; i++) {
			cell_array[i].resize(v_steps);
			border_heights[i*2].resize(v_steps);
			border_heights[i*2+1].resize(v_steps);
		}

		#ifdef QT_GUI_LIB
		QProgressDialog dialog(QString("Performing Raycasts..."), QString(), 0, v_steps);
		dialog.open();
		#endif
		
		//Perform all raycasts
		bool even_row = true;
		for (int v = 0; v < v_steps; v++) {

			#ifdef QT_GUI_LIB
			dialog.setValue(v);
			#endif
			
			double u_offset = odd_row_offset*static_cast<double>(even_row);
			double v_pos = start_v+static_cast<double>(v)*cell_half_height;
			
			for (int u = 0; u < u_steps; u++) {

				double u_pos = start_u+u_offset+static_cast<double>(u)*cell_u_division;

				//if (v > 0 && v < v_steps-1) {
					vector<double> heights;
					heightsAtPoint(u_pos, v_pos, heights);

					for (unsigned int i = 0; i < heights.size(); i++) {
						Point cell_point = Point::point2D(u_pos, v_pos, PaintCell::PAINT_AXIS);
						cell_point.setCoord(heights[i], PaintCell::PAINT_AXIS);
						cell_array[u][v].push_back(new PaintCell(cell_point));
					}
				//}

				 heightsAtPoint(u_pos-cell_radius, v_pos, border_heights[u*2][v]);
				 heightsAtPoint(u_pos+cell_radius, v_pos, border_heights[u*2+1][v]);
			}

			even_row = !even_row;
		}

		#ifdef QT_GUI_LIB
		dialog.setLabelText(QString("Creating Cells..."));
		#endif

		//Set all cell vertices
		even_row = true;
		for (int v = 0; v < v_steps; v++) {

			#ifdef QT_GUI_LIB
			dialog.setValue(v);
			#endif

			for (int u = 0; u < u_steps; u++) {

				for (unsigned int i = 0; i < cell_array[u][v].size(); i++) {

					PaintCell* cell = cell_array[u][v][i];

					bool found_all = true;
					for (int j = 0; j < PaintCell::CELL_VERTICES && found_all; j++) {

						int vertex_u; 
						int vertex_v;

						switch (j) {
							case 0:
								vertex_u = (u*2)+1;
								vertex_v = v;
								break;
							case 1:
								if (even_row)
									vertex_u = u*2;
								else
									vertex_u = (u+1)*2;
									
								vertex_v = v+1;
								break;
							case 2:
								if (even_row)
									vertex_u = (u-1)*2+1;
								else
									vertex_u = (u*2)+1;
								vertex_v = v+1;
								break;
							case 3:
								vertex_u = (u*2);
								vertex_v = v;
								break;
							case 4:
								if (even_row)
									vertex_u = (u-1)*2+1;
								else
									vertex_u = (u*2)+1;
									
								vertex_v = v-1;
								break;
							case 5:
								if (even_row)
									vertex_u = u*2;
								else
									vertex_u = (u+1)*2;
									
								vertex_v = v-1;
								break;
							default:
								break;

						}

						bool height_found = false;

						if (vertex_u >= 0 && vertex_u < u_steps*2 &&
							vertex_v >= 0 && vertex_v < v_steps) {

								vector<double> heights = border_heights[vertex_u][vertex_v];

								double wanted_height = cell->center.getCoord(PaintCell::PAINT_AXIS);
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
									Point wanted_point = cell->calcVertexLocation(j);
									wanted_point.setCoord(closest_height, PaintCell::PAINT_AXIS);
									cell->setVertex(j, wanted_point);
								}

						}

						found_all = (found_all && height_found);

					}

					if (found_all)
						paintCells.push_back(cell);
					else
						delete(cell);

				}

			}

			even_row = !even_row;
		}

		#ifdef QT_GUI_LIB
		dialog.setLabelText(QString("Finalizing..."));
		dialog.setRange(0, paintCells.size());
		#endif

		Point translation;
		translation.setCoord(PAINT_CELL_LIFT_AMOUNT, PaintCell::PAINT_AXIS);

		//Finalize the paint cells
		for (unsigned int i = 0; i < paintCells.size(); i++) {
			#ifdef QT_GUI_LIB
			dialog.setValue(i);
			#endif
			//paintCells[i]->fillEmptyVertices();
			//paintCells[i]->setBoundingDimensions();
			paintCells[i]->calculateNormal();
			paintCells[i]->translateVertices(translation);
			paintCells[i]->contractVertices(PAINT_CELL_CONTRACTION);
		}

		#ifdef QT_GUI_LIB
		dialog.close();
		#endif

		for (unsigned int i = 0; i < triangles.size(); i++)
			delete triangles[i];
		delete triangleTree;

		/*
        
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

				Point wanted_point = paintCells[i]->calcVertexLocation(v, cell_radius);

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
        
        LOG(PAINT, "Deleting partial paint cells ...");
        
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

		*/

	}

	void PaintGenerator::heightsAtPoint(double u, double v, vector<double>& vec) {

		Point p = Point::point2D(u, v, PaintCell::PAINT_AXIS);
		p.setCoord(paintBound.minCoord(PaintCell::PAINT_AXIS)-1.0, PaintCell::PAINT_AXIS);

		Point d;
		d.setCoord(1.0, PaintCell::PAINT_AXIS);
		RayIntersection intersect;

		do {
			Ray r(p, d, 0.0000001);
			intersect = triangleTree->rayIntersection(r);
			if (intersect.intersects) {
				p = r.atT(intersect.t);
				vec.push_back(p.getCoord(PaintCell::PAINT_AXIS));
			}

		} while (intersect.intersects);

		/*

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

		*/
	}

}  // namespace Paint
}  // namespace Project
