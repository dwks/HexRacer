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
		Math::HexHeightMap& heightMap,
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

		#ifndef USE_BULLET_RAYCASTS

		vector<ObjectSpatial*> triangles;

		paintBound.setToObject(_triangles[0]);
		triangles.push_back(new Triangle3D(_triangles[0]));
		for (unsigned int i = 1; i < _triangles.size(); i++) {
			paintBound.expandToInclude(_triangles[i]);
			triangles.push_back(new Triangle3D(_triangles[i]));
		}

		triangleTree = new BSPTree3D(paintBound);
		triangleTree->add(triangles);

		#endif

		Math::HexGrid hex_grid = heightMap.getHexGrid();

		double cell_radius = hex_grid.getHexRadius();

		vector< vector< vector<PaintCell*> > > cell_array;

		int u_steps = hex_grid.numUIndices();
		int v_steps = hex_grid.numVIndices();

		cell_array.resize(u_steps);
		for (int i = 0; i < u_steps; i++) {
			cell_array[i].resize(v_steps);
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
				
				
				vector<double> vert_heights1;
				vector<double> vert_heights2;

				heightsAtPoint(u_pos-cell_radius, v_pos, vert_heights1);

				for (unsigned int i = 0; i < vert_heights1.size(); i++) 
					heightMap.addVertexHeight(u*2, v, vert_heights1[i]);

				heightsAtPoint(u_pos+cell_radius, v_pos, vert_heights2);

				for (unsigned int i = 0; i < vert_heights2.size(); i++) 
					heightMap.addVertexHeight(u*2+1, v, vert_heights2[i]);

				if (!vert_heights1.empty() && !vert_heights2.empty()) {

					vector<double> center_heights;
					heightsAtPoint(u_pos, v_pos, center_heights);

					for (unsigned int i = 0; i < center_heights.size(); i++)
						heightMap.addHexHeight(u, v, center_heights[i]);
				}
				
			}

		}

		#ifndef USE_BULLET_RAYCASTS

		for (unsigned int i = 0; i < triangles.size(); i++)
			delete triangles[i];
		delete triangleTree;

		#endif
	}

	void PaintGenerator::generateCells(const Math::HexHeightMap& heightMap,
		std::vector<PaintCellInfo>& cellInfo,
		Misc::ProgressTracker* progress_tracker) {

		const vector<HexGrid::HexIndex>& hex_indices = heightMap.getSetHexIndices();

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Creating paint cells...");
			progress_tracker->setTotalSteps(static_cast<int>(hex_indices.size()));
		}

		for (unsigned int i = 0; i < hex_indices.size(); i++) {

			if (progress_tracker)
				progress_tracker->setCurrentStep(i);

			const std::list<double>* heights = heightMap.getHexHeights(hex_indices[i]);

			for (unsigned int j = 0; j < heights->size(); j++) {
				PaintCellInfo info(hex_indices[i].uIndex, hex_indices[i].vIndex);
				if (info.calcLayerIndices(heightMap, j, PAINT_CELL_VERTEX_HEIGHT_THRESHHOLD)) {
					info.calcNormal(heightMap);
					cellInfo.push_back(info);
				}
			}

		}
	}

	bool PaintGenerator::heightsAtPoint(double u, double v, vector<double>& vec) {

		bool hit = false;

		Point p = Point::point2D(u, v, PaintCell::PAINT_AXIS);
		p.setCoord(paintBound.minCoord(PaintCell::PAINT_AXIS)-1.0, PaintCell::PAINT_AXIS);

		Point d;
		d.setCoord(1.0, PaintCell::PAINT_AXIS);
		RayIntersection intersect;

		#ifdef USE_BULLET_RAYCASTS
			std::vector<Math::Point> data;
			Physics::PhysicsWorld::getInstance()->allRaycastPoints(p, p+d*100000, data);
			for (int x = 0; x < int(data.size()); x ++) {
				vec.push_back(data[x].getCoord(PaintCell::PAINT_AXIS));
				hit = true;
			}
		#else
		do {
			Ray r(p, d, 0.0000001);
			intersect = triangleTree->rayIntersection(r);
			if (intersect.intersects) {
				p = r.atT(intersect.t);
				vec.push_back(p.getCoord(PaintCell::PAINT_AXIS));
				hit = true;
			}

		} while (intersect.intersects);
#endif
		return hit;
	}

}  // namespace Paint
}  // namespace Project
