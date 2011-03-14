#ifndef PROJECT_PAINT__PAINT_MANAGER_H
#define PROJECT_PAINT__PAINT_MANAGER_H

#include <vector>

#include "math/HexGrid.h"
#include "math/HexHeightMap.h"
#include "PaintCell.h"
#include "PaintGrid.h"
#include "PaintCellInfo.h"

#include "render/BaseRenderable.h"
#include "math/BSPTree3D.h"
#include "math/BoundingBox2D.h"
#include "math/SpatialObjectOperator.h"
#include "map/HRMap.h"

#include "event/PaintEvent.h"
#include "event/PaintCellsChanged.h"
#include "event/Enabler.h"

#include "misc/ProgressTracker.h"

#include "settings/SettingsManager.h"

namespace Project {
namespace Paint {

class PaintManager :
    public Render::BaseRenderable, public Math::SpatialObjectOperator,
    public Event::Enabler {

protected:
    void paintEventHandler(Event::PaintEvent *paintEvent);
    void paintCellsChangedHandler(Event::PaintCellsChanged *paintCellsChanged);

private:

	std::vector<PaintCell*> paintList;
	Math::BSPTree3D* coloredPaintTree;
	float fadePlaneNear;
	float fadePlaneFar;
	GLuint targetList;
	bool enableRendering;

	Math::HexGrid hexGrid;
	PaintGrid paintGrid;
	Map::HRMap* _map;

	std::vector<PaintCell*> redrawBuffer;

	static const int TREE_SPLIT_SIZE = 30;
	static const Math::BSPTree3D::SplitMethod TREE_SPLIT_METHOD = Math::BSPTree3D::LARGEST_AXIS;

	bool colorCell(PaintCell* cell, int new_color, bool force_color = false);
	void renderCell(PaintCell* cell);
	void renderCellMinimap(PaintCell* cell);

	double paintCellArea;

public:

	struct ColorIndex {
		int color;
		int index;
	};

	PaintManager(bool enable_rendering = true);
	~PaintManager();

	void setMap(Map::HRMap* map, Misc::ProgressTracker* progress_tracker = NULL);
    
	void renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object, const Render::RenderSettings& settings);

	void minimapRender(const Math::BoundingObject2D& bounding_object, float view_height, float alpha = 1.0f);

	void setFadePlanes(float near_plane, float far_plane) {
		fadePlaneNear = near_plane;
		fadePlaneFar = far_plane;
	}


	void colorCellByIndex(const Math::HexHeightMap::LayeredHexIndex& index, int new_color, bool force_color = false);

	void colorCellsInRadius(Math::Point centroid, double radius, int new_color, bool force_color = false, vector<Math::HexHeightMap::LayeredHexIndex>* changedIndices = NULL);

	void renderEraseEffect(Math::Point centroid, double radius);

	double weightedCellsInRadius(Math::Point centroid, double radius, int color);

	void operateOnObject(Math::ObjectSpatial* object);
	
};

}  // namespace Paint
}  // namespace Project

#endif
