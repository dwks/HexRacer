#include "PaintGenerator.h"

#include "boost/smart_ptr.hpp"

#include "math/BoundingBox2D.h"
#include "math/Vertex2D.h"
#include "math/Geometry.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "config.h"

#include <cmath>

#define USE_BULLET_RAYCASTS

#ifdef QT_GUI_LIB
#include <QtGui>
#endif
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Paint {

	PaintGenerator::PaintGenerator() {
		triangleTree = NULL;		
	}

	void PaintGenerator::generateHeightmap(const std::vector<Math::Triangle3D>& _triangles,
		const HexGrid& hex_grid,
		Misc::ProgressTracker* progress_tracker) {

		if (_triangles.size() <= 0)
			return;

		#ifdef USE_BULLET_RAYCASTS
			// bullet initialization
			boost::shared_ptr<Physics::PhysicsWorld> physicsWorld(
				new Physics::PhysicsWorld());
			btRigidBody *meshBody
				= Physics::PhysicsFactory::createRigidTriMesh(_triangles);
			physicsWorld->registerRigidBody(meshBody);
		#endif
			
		heightMap = HexHeightMap(hex_grid);

		vector<ObjectSpatial*> triangles;

		paintBound.setToObject(_triangles[0]);
		triangles.push_back(new Triangle3D(_triangles[0]));
		for (unsigned int i = 1; i < _triangles.size(); i++) {
			paintBound.expandToInclude(_triangles[i]);
			triangles.push_back(new Triangle3D(_triangles[i]));
		}

		#ifndef USE_BULLET_RAYCASTS

		triangleTree = new BSPTree3D(paintBound);
		triangleTree->add(triangles);

		#else

		triangleTree = NULL;

		#endif

		double cell_radius = hex_grid.getHexRadius();

		vector< vector< vector<PaintCell*> > > cell_array;
		vector< vector< vector<double> > > border_heights;

		int u_steps = hex_grid.numUIndices();
		int v_steps = hex_grid.numVIndices();

		cell_array.resize(u_steps);
		border_heights.resize(u_steps*2);
		for (int i = 0; i < u_steps; i++) {
			cell_array[i].resize(v_steps);
			border_heights[i*2].resize(v_steps);
			border_heights[i*2+1].resize(v_steps);
		}

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Generating heightmap...");
			progress_tracker->setTotalSteps(v_steps);
		}

		//Perform all raycasts
		for (int v = 0; v < v_steps; v++) {

			if (progress_tracker)
				progress_tracker->setCurrentStep(v);
			
			double v_pos = hex_grid.hexVPosition(v);
			
			for (int u = 0; u < u_steps; u++) {

				double u_pos = hex_grid.hexUPosition(u, v);
				
				vector<double> heights;
				heightsAtPoint(u_pos, v_pos, heights);

				for (unsigned int i = 0; i < heights.size(); i++)
					heightMap.addHexHeight(u, v, heights[i]);

				heights.clear();
				heightsAtPoint(u_pos-cell_radius, v_pos, heights);
				for (unsigned int i = 0; i < heights.size(); i++) 
					heightMap.addVertexHeight(u*2, v, heights[i]);

				heights.clear();
				heightsAtPoint(u_pos+cell_radius, v_pos, heights);
				for (unsigned int i = 0; i < heights.size(); i++) 
					heightMap.addVertexHeight(u*2+1, v, heights[i]);
			}

		}

		for (unsigned int i = 0; i < triangles.size(); i++)
			delete triangles[i];
		delete triangleTree;
	}

	void PaintGenerator::generateCells(Misc::ProgressTracker* progress_tracker) {

		int u_steps = heightMap.getHexGrid().numUIndices();
		int v_steps = heightMap.getHexGrid().numVIndices();

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Creating paint cells...");
			progress_tracker->setTotalSteps(v_steps);
		}

		for (int v = 0; v < v_steps; v++) {

			progress_tracker->setCurrentStep(v);

			for (int u = 0; u < u_steps; u++) {

				const std::list<double>* heights = heightMap.getHexHeights(u, v);
				if (heights) {
					for (unsigned int i = 0; i < heights->size(); i++) {
						PaintCellInfo info(u, v);
						if (info.calcLayerIndices(heightMap, i, PAINT_CELL_VERTEX_HEIGHT_THRESHHOLD)) {
							info.calcNormal(heightMap);
							cellInfo.push_back(info);
						}
					}
				}

			}

		}
	}

	void PaintGenerator::heightsAtPoint(double u, double v, vector<double>& vec) {

		Point p = Point::point2D(u, v, PaintCell::PAINT_AXIS);
		p.setCoord(paintBound.minCoord(PaintCell::PAINT_AXIS)-1.0, PaintCell::PAINT_AXIS);

		Point d;
		d.setCoord(1.0, PaintCell::PAINT_AXIS);
		RayIntersection intersect;

		#ifdef USE_BULLET_RAYCASTS
			std::vector<Math::Point> data;
			Physics::PhysicsWorld::getInstance()->allRaycastPoints(p, p+d*100000, data);
			for(int x = 0; x < int(data.size()); x ++) {
				vec.push_back(data[x].getCoord(PaintCell::PAINT_AXIS));
			}
		#else
		do {
			Ray r(p, d, 0.0000001);
			intersect = triangleTree->rayIntersection(r);
			if (intersect.intersects) {
				p = r.atT(intersect.t);
				vec.push_back(p.getCoord(PaintCell::PAINT_AXIS));
			}

		} while (intersect.intersects);
#endif
	}

}  // namespace Paint
}  // namespace Project
