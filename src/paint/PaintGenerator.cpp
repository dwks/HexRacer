#include "PaintGenerator.h"

#include "boost/smart_ptr.hpp"

#include "math/BoundingBox2D.h"
#include "math/Vertex2D.h"
#include "math/Geometry.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "config.h"

#include <cmath>

#undef QT_GUI_LIB

#ifdef QT_GUI_LIB
#include <QtGui>
#endif

#define USE_BULLET_RAYCASTS

using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Paint {

PaintGenerator::PaintGenerator(const std::vector<Triangle3D> &_triangles,
    const Math::HexGrid& grid) : heightMap(grid) {
    
#ifdef USE_BULLET_RAYCASTS
    // bullet initialization
    boost::shared_ptr<Physics::PhysicsWorld> physicsWorld(
        new Physics::PhysicsWorld());
    btRigidBody *meshBody
        = Physics::PhysicsFactory::createRigidTriMesh(_triangles);
    physicsWorld->registerRigidBody(meshBody);
#endif
    
    if (_triangles.size() <= 0)
        return;

    vector<ObjectSpatial*> triangles;

    paintBound.setToObject(_triangles[0]);
    triangles.push_back(new Triangle3D(_triangles[0]));
    for (unsigned int i = 1; i < _triangles.size(); i++) {
        paintBound.expandToInclude(_triangles[i]);
        triangles.push_back(new Triangle3D(_triangles[i]));
    }

    triangleTree = new BSPTree3D(paintBound);
    triangleTree->add(triangles);

    double cell_radius = grid.getHexRadius();

    vector< vector< vector<PaintCell*> > > cell_array;
    vector< vector< vector<double> > > border_heights;

    int u_steps = grid.numUIndices();
    int v_steps = grid.numVIndices();

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
    for (int v = 0; v < v_steps; v++) {

        #ifdef QT_GUI_LIB
        dialog.setValue(v);
        #endif
        
        double v_pos = grid.hexVPosition(v);
        
        for (int u = 0; u < u_steps; u++) {

            double u_pos = grid.hexUPosition(u, v);

            vector<double> heights;
            heightsAtPoint(u_pos, v_pos, heights);

            for (unsigned int i = 0; i < heights.size(); i++) {
                Point cell_point = Point::point2D(u_pos, v_pos, PaintCell::PAINT_AXIS);
                cell_point.setCoord(heights[i], PaintCell::PAINT_AXIS);
                cell_array[u][v].push_back(new PaintCell(cell_point));
            }

            heightsAtPoint(u_pos-cell_radius, v_pos, border_heights[u*2][v]);
            heightsAtPoint(u_pos+cell_radius, v_pos, border_heights[u*2+1][v]);
        }

    }

    #ifdef QT_GUI_LIB
    dialog.setLabelText(QString("Creating Cells..."));
    #endif

    //Set all cell vertices
    for (int v = 0; v < v_steps; v++) {

        #ifdef QT_GUI_LIB
        dialog.setValue(v);
        #endif

        for (int u = 0; u < u_steps; u++) {

            for (unsigned int i = 0; i < cell_array[u][v].size(); i++) {

                PaintCell* cell = cell_array[u][v][i];

                bool found_all = true;
                for (int j = 0; j < PaintCell::CELL_VERTICES && found_all; j++) {

                    Math::HexGrid::HexIndex vert_index = grid.vertexIndex(u, v, j);

                    bool height_found = false;

                    if (grid.validVertex(vert_index)) {

                        //list<double>* heights = heightMap.get();
                        vector<double> heights = border_heights[vert_index.uIndex][vert_index.vIndex];

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
                            //Point wanted_point = cell->calcVertexLocation(j);
                            HexGrid::HexPosition pos = grid.vertexPosition(vert_index);
                            Point wanted_point = Math::Point::point2D(pos.uPos, pos.vPos, PaintCell::PAINT_AXIS);
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
