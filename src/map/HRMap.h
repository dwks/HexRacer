#ifndef PROJECT_MAP__MAP_FILE_H
#define PROJECT_MAP__MAP_FILE_H

#include "render/MeshLoader.h"
#include "render/RenderList.h"
#include "render/TextureCube.h"
#include "render/CubeMapFile.h"
#include "render/Light.h"
#include "paint/PaintCell.h"
#include "PathNode.h"
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
	Render::CubeMapFile* cubeMapFile;
	Render::TextureCube* cubeMap;
	std::vector<Render::Light*> lights;
	std::vector<PathNode*> pathNodes;
	std::vector<Paint::PaintCell*> paintCells;
	std::vector<Math::Vertex3D*> startPoints;

	Math::BSPTree3D* collisionTree;

public:

	static const int NUM_MESHES = 5;

	enum MeshType {TRACK, INVIS_TRACK, SOLID, INVIS_SOLID, DECOR};

	HRMap();
	~HRMap();
	bool loadMapFile(std::string _filename);
	bool saveMapFile(std::string _filename) const;
	Render::MeshGroup* getMapMesh(MeshType type) const { return mapMesh[static_cast<int>(type)]; }
	Render::RenderList* getTrackRenderable() const { return trackRenderable; }
	Render::TextureCube* getCubeMap();
	const Render::CubeMapFile* getCubeMapFile() const { return cubeMapFile; }
	void setCubeMapFile(const Render::CubeMapFile& file);
	void clearCubeMap();
	const std::vector<Paint::PaintCell*>& getPaintCells() const { return paintCells; }
	Math::BSPTree3D* getCollisionTree();
	void clear();
	void clearPaint();
	void loadMapMesh(HRMap::MeshType type, string filename);
	void clearMapMesh(HRMap::MeshType type);
	void generatePaint(double cell_radius = PAINT_CELL_RADIUS);

	const std::vector<Render::Light*>& getLights() const { return lights; }
	void addLight(Render::Light* light);
	void removeLight(Render::Light* light);
	void clearLights();
	const std::vector<PathNode*>& getPathNodes() const { return pathNodes; }
	void addPathNode(PathNode* node);
	void removePathNode(PathNode* node);
	void clearPathNodes();
	const std::vector<Math::Vertex3D*>& getStartPoints() const { return startPoints; }
	void addStartPoint(Math::Vertex3D* point);
	void removeStartPoint(Math::Vertex3D* point);
	void clearStartPoints();

	std::string getFilename() const { return filename; }

	static std::string meshName(MeshType type);
	static std::string meshTitle(MeshType type);

private:

	void clearCollisionTree();

};

}  // namespace Map
}  // namespace Project

#endif
