#ifndef PROJECT_MAP__MAP_FILE_H
#define PROJECT_MAP__MAP_FILE_H

#include "render/MeshLoader.h"
#include "render/RenderList.h"
#include "render/TextureCube.h"
#include "render/CubeMapFile.h"
#include "render/MeshGroup.h"
#include "render/Light.h"
#include "render/TransformedMesh.h"
#include "paint/PaintCell.h"
#include "math/BoundingPlane3D.h"
#include "PathNode.h"
#include "MeshInstance.h"
#include <string>
#include <vector>

#define HRMAP_NUM_MESHES 5
#define HRMAP_VERSION "0.0.2"
#define HRMAP_VERSION_LABEL "version"
#define HRMAP_PROPMESH_LABEL "propMesh"
#define HRMAP_LIGHT_LABEL "light"
#define HRMAP_PATHNODE_LABEL "pathNode"
#define HRMAP_STARTPOINT_LABEL "startPoint"
#define HRMAP_MESHINSTANCE_LABEL "meshInstance"
#define HRMAP_FINISHPLANE_LABEL "finishPlane"
#define HRMAP_PAINTCELL_LABEL "paintCell"
#define HRMAP_MAP2DFILE_LABEL "map2DFile"
#define HRMAP_MAP2DCENTER_LABEL "map2DCenter"
#define HRMAP_MAP2DWIDTH_LABEL "map2DWidth"
#define HRMAP_MAP2DHEIGHT_LABEL "map2DHeight"

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
	std::vector<MeshInstance*> meshInstances;
	Math::BoundingPlane3D finishPlane;

	std::vector<std::string> propMeshNames;
	std::vector<std::string> propMeshFilenames;

	std::string map2DFile;
	Math::Point map2DCenter;
	double map2DWidth;
	double map2DHeight;

	Math::BSPTree3D* collisionTree;

public:

	enum MeshType {TRACK, INVIS_TRACK, SOLID, INVIS_SOLID, DECOR, NUM_MESHES};

	HRMap();
	~HRMap();
	bool loadMapFile(std::string _filename);
	bool saveMapFile(std::string _filename);
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

	std::string getMap2DFile() const { return map2DFile; }
	Math::Point getMap2DCenter() const { return map2DCenter; }
	double getMap2DWidth() const { return map2DWidth; }
	double getMap2DHeight() const { return map2DHeight; }

	void setMap2DFile(std::string file) { map2DFile = file; }
	void setMap2DCenter(Math::Point center) { map2DCenter = center; }
	void setMap2DWidth(double width) { map2DWidth = width; }
	void setMap2DHeight(double height) { map2DHeight = height; }

	bool addPropMesh(std::string name, std::string filename);
	bool removePropMesh(int index);
	std::vector<std::string> getPropMeshNames() { return propMeshNames; }
	std::string getPropMeshName(int index);

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
	Math::BoundingPlane3D& getFinishPlane() { return finishPlane; }

	const std::vector<MeshInstance*> getMeshInstances() const { return meshInstances; }
	bool addMeshInstance(MeshInstance* mesh);
	void removeMeshInstance(MeshInstance* mesh);
	void clearMeshInstances();

	std::string getFilename() const { return filename; }

	static std::string meshName(MeshType type);
	static std::string meshTitle(MeshType type);
	static bool meshIsInvisible(MeshType type);
	static bool meshIsSolid(MeshType type);

private:

	void clearCollisionTree();

};

}  // namespace Map
}  // namespace Project

#endif
