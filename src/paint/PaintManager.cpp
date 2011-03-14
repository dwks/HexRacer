#include "PaintManager.h"
#include "opengl/MathWrapper.h"
#include "math/BoundingSphere.h"
#include "math/BoundingCircle.h"
#include "render/ColorConstants.h"
#include "event/EventSystem.h"

using namespace Project;
using namespace Math;
using namespace Render;

namespace Project {
namespace Paint {

    void PaintManager::paintEventHandler(Event::PaintEvent *paintEvent) {
        Math::Point position = paintEvent->getPoint();
        double radius = paintEvent->getRadius();
        int colour = paintEvent->getColour();
        
        colorCellsInRadius(position, radius, colour);
    }
    
    void PaintManager::paintCellsChangedHandler(
        Event::PaintCellsChanged *paintCellsChanged) {
        
		/*
        colorCellsByIndex(
            paintCellsChanged->getCells(),
            paintCellsChanged->getColour());
		*/
    }
    
	PaintManager::PaintManager(bool enable_rendering) {

		coloredPaintTree = new BSPTree3D(BoundingBox3D(), TREE_SPLIT_METHOD, TREE_SPLIT_SIZE);
		getRenderProperties()->setWantsShaderName("paintShader");
		OpenGL::Material* material = new OpenGL::Material("paintMaterial");
		material->setShininess(4.0f);
		getRenderProperties()->setMaterial(material);

		enableRendering = enable_rendering;

		fadePlaneNear = 0.0f;
		fadePlaneFar = 0.0f;
		targetList = 0;
		paintCellArea = 0.5;
        
        METHOD_OBSERVER(&PaintManager::paintEventHandler);
        METHOD_OBSERVER(&PaintManager::paintCellsChangedHandler);
	}

	PaintManager::~PaintManager() {
		delete(coloredPaintTree);
		delete(getRenderProperties()->getMaterial());
		for (unsigned int i = 0; i < paintList.size(); i++) {
			delete paintList[i];
		}
    }

	void PaintManager::setMap(Map::HRMap* map, Misc::ProgressTracker* progress_tracker) {
		
		hexGrid = map->getHexGrid();
		paintGrid = PaintGrid(hexGrid);

		paintCellArea = PI*hexGrid.getHexRadius()*hexGrid.getHexRadius();

		const vector<PaintCellInfo>& cell_info = map->getPaintCellInfo();

		BoundingBox3D paintBound;

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Creating paint cells...");
			progress_tracker->setTotalSteps(static_cast<int>(cell_info.size()));
		}

		for (unsigned int i = 0; i < cell_info.size(); i++) {

			if (progress_tracker)
				progress_tracker->setCurrentStep(static_cast<int>(i));

			const PaintCellInfo& cell = cell_info[i];

			PaintCell* new_cell = new PaintCell(cell.centerPoint(map->getPaintHeightMap()));

			if (enableRendering) {

				new_cell->displayList = glGenLists(1);
				glNewList(new_cell->displayList, GL_COMPILE);
				cell.render(map->getPaintHeightMap());
				glEndList();

				new_cell->setToObject(cell.getBoundingBox(map->getPaintHeightMap()));
				if (i == 0)
					paintBound.setToObject(*new_cell);
				else
					paintBound.expandToInclude(*new_cell);

			}

			paintGrid.addPaintCell(cell.uIndex, cell.vIndex, new_cell);
			paintList.push_back(new_cell);
		}

		coloredPaintTree->resize(paintBound);

	}

	void PaintManager::renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object, const Render::RenderSettings& settings) {

		GLfloat fade_planes [2] = {fadePlaneNear, fadePlaneFar};
		setter.setParamFloatArray(
			Shader::ShaderParameter::UNIFORM,
			"fadePlanes",
			fade_planes,
			2);

		if (!settings.getRedrawMode()) {
			redrawBuffer.clear();
			if (bounding_object)
				coloredPaintTree->operateQuery(*this, *bounding_object, SpatialContainer::NEARBY);
			else
				coloredPaintTree->operateAll(*this);

		}
		else {
			for (unsigned int i = 0; i < redrawBuffer.size(); i++) {
				renderCell(redrawBuffer[i]);
			}
		}

	}

	void PaintManager::minimapRender(const Math::BoundingObject2D& bounding_object, float view_height, float alpha) {

		GLfloat values [4];
		glGetFloatv(GL_VIEWPORT, values);
		float viewportWidth = values[3];
		glPointSize(viewportWidth/view_height);
		glBegin(GL_POINTS);

		HexGrid::HexIndexRange range = hexGrid.hexIndexRange(bounding_object);

		if (hexGrid.validRange(range)) {
			for (int u = range.minUIndex; u <= range.maxUIndex; u++) {
				for (int v = range.minVIndex; v <= range.maxVIndex; v++) {
					
					PaintCell* cell = paintGrid.getPaintCell(u, v);
					while (cell != NULL) {
						if (cell->playerColor >= 0) {
							OpenGL::Color::glColor(ColorConstants::playerColor(cell->playerColor), alpha);
							OpenGL::MathWrapper::glVertex(cell->position);
						}
						cell = cell->nextCell;
					}
				}
			}
		}

		glEnd();
		glPointSize(1.0f);
		

	}

	void PaintManager::colorCellByIndex(const HexHeightMap::LayeredHexIndex& index, int new_color, bool force_color) {
		PaintCell* cell = paintGrid.getPaintCell(index.hexIndex);
		int layer = index.layerIndex;
		while (layer > 0 && cell) {
			cell = cell->nextCell;
			layer--;
		}
		if (cell)
			colorCell(cell, new_color, force_color);
	}

	void PaintManager::colorCellsInRadius(Math::Point centroid, double radius, int new_color, bool force_color, vector<HexHeightMap::LayeredHexIndex>* changedIndices) {

		double radius_squared = radius*radius;
		BoundingCircle query_circle(centroid, radius, PaintCell::PAINT_AXIS);
		HexGrid::HexIndexRange range = hexGrid.hexIndexRange(query_circle);

		if (hexGrid.validRange(range)) {
			for (int u = range.minUIndex; u <= range.maxUIndex; u++) {
				for (int v = range.minVIndex; v <= range.maxVIndex; v++) {
					
					PaintCell* cell = paintGrid.getPaintCell(u, v);
					int layer = 0;
					while (cell != NULL) {
						if (centroid.distanceSquared(cell->position) <= radius_squared) {
							if (colorCell(cell, new_color, force_color) && changedIndices) {
								HexHeightMap::LayeredHexIndex index;
								index.hexIndex.uIndex = u;
								index.hexIndex.vIndex = v;
								index.layerIndex = layer;
								changedIndices->push_back(index);
							}
						}
						cell = cell->nextCell;
						layer++;
					}

				}
			}
		}

	}

	double PaintManager::weightedCellsInRadius(Point centroid, double radius, int color) {

		double weighted_score = 0.0;
		
		//Scale the score weighting according to the amount of cells that can be inside the radius
		double density_factor = paintCellArea/(PI*radius*radius);

		double color_benefit = GET_SETTING("game.paint.colorbenefit", 1.0);
		double color_penalty = GET_SETTING("game.paint.colorpenalty", 0.0);

		double radius_squared = radius*radius;
		BoundingCircle query_circle(centroid, radius, PaintCell::PAINT_AXIS);
		HexGrid::HexIndexRange range = hexGrid.hexIndexRange(query_circle);

		if (hexGrid.validRange(range)) {
			for (int u = range.minUIndex; u <= range.maxUIndex; u++) {
				for (int v = range.minVIndex; v <= range.maxVIndex; v++) {
					
					PaintCell* cell = paintGrid.getPaintCell(u, v);
					while (cell != NULL) {
						double distance_squared = centroid.distanceSquared(cell->position);
						if (distance_squared <= radius_squared) {

							double distance_factor = 1.0-(distance_squared/radius_squared);
							
							if (cell->playerColor == color)
								weighted_score += color_benefit*distance_factor;
							else if (cell->playerColor >= 0)
								weighted_score += color_penalty*distance_factor;

						}
						cell = cell->nextCell;
					}
				}
			}
		}

		return weighted_score*density_factor;

		/*
		double weighted_score = 0.0;
        int cell_count = 0;
        
		BoundingSphere query_sphere(centroid, radius);
		vector<ObjectSpatial*> candidate_cells;
		coloredPaintTree->appendQuery(candidate_cells, query_sphere, SpatialContainer::NEARBY);

		double color_benefit = GET_SETTING("game.paint.colorbenefit", 1.0);
		double color_penalty = GET_SETTING("game.paint.colorpenalty", 0.0);
        
		for (unsigned int i = 0; i < candidate_cells.size(); i++) {
			PaintCell *cell = static_cast<PaintCell *>(candidate_cells[i]);
			double dist_squared = cell->center.distanceSquared(centroid);
            
			if (dist_squared <= query_sphere.getRadiusSquared()) {

				double dist = sqrt(dist_squared);

                cell_count ++;
                double colour_factor;
                
                if(cell->playerColor == color) {
                    colour_factor = color_benefit;
                }
                else if(cell->playerColor < 0) {
                    colour_factor = 0.0;
                    continue;
                }
                else {
                    colour_factor = color_penalty;
                }
                
                double distance_weight = (query_sphere.getRadius() - dist)
                    / query_sphere.getRadius();
                
                weighted_score += colour_factor * distance_weight;
			}
		}
        
        if(!cell_count) {
            return 1.0;
        }
        else {
            return 1.0 + weighted_score / cell_count;
        }
		*/

		return 1.0;
	}

	bool PaintManager::colorCell(PaintCell* cell, int new_color, bool force_color) {

		if (cell->playerColor == new_color)
			return false;

		if (new_color >= 0) {
			
			if (cell->playerColor < 0) {

				cell->playerColor = new_color;
				if (enableRendering)
					coloredPaintTree->add(cell);
				return true;

			}
			else if (force_color) {
				cell->playerColor = new_color;
				return true;
			}

		}

		if (new_color < 0) {
			
			if (cell->playerColor >= 0) {

				cell->playerColor = new_color;
				if (enableRendering)
					coloredPaintTree->remove(cell);
				return true;

			}
			else if (force_color) {
				cell->playerColor = new_color;
				return true;
			}

		}

		return false;
	}

	void PaintManager::operateOnObject(Math::ObjectSpatial* object) {
		renderCell((PaintCell*)object);
		redrawBuffer.push_back((PaintCell*)object);
	}

	void PaintManager::renderCell(PaintCell* cell) {

		OpenGL::Color cell_color;

		if (cell->playerColor >= 0)
			cell_color = ColorConstants::playerColor(cell->playerColor);
		else {
			cell_color = OpenGL::Color::WHITE;
			cell_color.setAlphaf(0.4f);
		}

		OpenGL::Color::glColor(cell_color);
		glCallList(cell->displayList);
	}

	void PaintManager::renderEraseEffect(Math::Point centroid, double radius) {

		double radius_squared = radius*radius;
		BoundingCircle query_circle(centroid, radius, PaintCell::PAINT_AXIS);
		HexGrid::HexIndexRange range = hexGrid.hexIndexRange(query_circle);

		if (hexGrid.validRange(range)) {
			for (int u = range.minUIndex; u <= range.maxUIndex; u++) {
				for (int v = range.minVIndex; v <= range.maxVIndex; v++) {
					
					PaintCell* cell = paintGrid.getPaintCell(u, v);
					while (cell != NULL) {
						if (cell->playerColor < 0 && centroid.distanceSquared(cell->position) <= radius_squared )
							renderCell(cell);
						cell = cell->nextCell;
					}

				}
			}
		}

	}

}  // namespace Paint
}  // namespace Project
