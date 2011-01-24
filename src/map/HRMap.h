#ifndef PROJECT_MAP__MAP_FILE_H
#define PROJECT_MAP__MAP_FILE_H

#include "render/MeshLoader.h"
#include "render/RenderList.h"
#include "render/TextureCube.h"
#include "render/Light.h"
#include "paint/PaintCell.h"
#include <string>
#include <vector>

#define HRMAP_NUM_MESHES 5

namespace Project {
namespace Map {

class HRMap {
private:

	Render::MeshGroup* mapMesh[HRMAP_NUM_MESHES];
	std::string mapMeshFile[HRMAP_NUM_MESHES];

	Render::RenderList* trackRenderable;
	std::string version;
	std::string filename;
	std::string bgImageXPos;
	std::string bgImageXNeg;
	std::string bgImageYPos;
	std::string bgImageYNeg;
	std::string bgImageZPos;
	std::string bgImageZNeg;
	Render::TextureCube* cubeMap;
	std::vector<Render::Light*> lights;
	std::vector<Paint::PaintCell*> paintCells;

public:

	static const int NUM_MESHES = 5;

	enum MeshType {TRACK, INVIS_TRACK, SOLID, INVIS_SOLID, DECOR};

	HRMap();
	~HRMap();
	bool loadMapFile(std::string _filename);
	bool saveMapFile(std::string _filename) const;
	Render::MeshGroup* getMapMesh(MeshType type) const { return mapMesh[static_cast<int>(type)]; }
	Render::RenderList* getTrackRenderable() const { return trackRenderable; }
	Render::TextureCube* getCubeMap() const { return cubeMap; }
	std::vector<Paint::PaintCell*> getPaintCells() const { return paintCells; }


	std::vector<Render::Light*> getLights() const { return lights; }
	void clear();
	void clearPaint();
	void loadMapMesh(HRMap::MeshType type, string filename);
	void generatePaint(double cell_radius = PAINT_CELL_RADIUS);

	std::string getFilename() const { return filename; }

	static std::string meshName(MeshType type);

};

}  // namespace Map
}  // namespace Project

#endif
