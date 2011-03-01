#ifndef PROJECT_MAP__MAP_FILE_H
#define PROJECT_MAP__MAP_FILE_H

#include "mesh/MeshLoader.h"
#include "render/RenderList.h"
#include "opengl/TextureCube.h"
#include "opengl/CubeMapFile.h"
#include "opengl/Light.h"
#include "mesh/TransformedMesh.h"
#include "mesh/MeshGroup.h"
#include "paint/PaintCell.h"
#include "math/BoundingPlane3D.h"
#include "math/HexGrid.h"
#include "PathNode.h"
#include "MeshInstance.h"
#include "MapOptions.h"
#include <string>
#include <vector>

#define HRMAP_NUM_MESHES 5
#define HRMAP_VERSION "0.1.5"
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

	Mesh::MeshGroup* mapMesh[HRMAP_NUM_MESHES];
	std::string mapMeshFile[HRMAP_NUM_MESHES];

	Render::RenderList* trackRenderable;
	std::string version;
	std::string filename;
	OpenGL::CubeMapFile* cubeMapFile;
	OpenGL::TextureCube* cubeMap;
	std::vector<OpenGL::Light*> lights;
	std::vector<PathNode*> pathNodes;
	std::vector<Paint::PaintCell*> paintCells;
	std::vector<Math::Vertex3D*> startPoints;
	std::vector<MeshInstance*> meshInstances;
	Math::BoundingPlane3D finishPlane;
	Math::BoundingBox3D mapBoundingBox;

	std::vector<std::string> propMeshNames;
	std::vector<std::string> propMeshFilenames;

	std::string map2DFile;
	Math::Point map2DCenter;
	double map2DWidth;
	double map2DHeight;

	MapOptions mapOptions;

	Math::BSPTree3D* collisionTree;
	Math::HexGrid hexGrid;

public:

	enum MeshType {TRACK, INVIS_TRACK, SOLID, INVIS_SOLID, DECOR, NUM_MESHES};

	HRMap();
	~HRMap();
	bool loadMapFile(std::string _filename);
	bool saveMapFile(std::string _filename);
	Mesh::MeshGroup* getMapMesh(MeshType type) const { return mapMesh[static_cast<int>(type)]; }
	Render::RenderList* getTrackRenderable() const { return trackRenderable; }
	OpenGL::TextureCube* getCubeMap();
	const OpenGL::CubeMapFile* getCubeMapFile() const { return cubeMapFile; }
	void setCubeMapFile(const OpenGL::CubeMapFile& file);
	void clearCubeMap();
	const std::vector<Paint::PaintCell*>& getPaintCells() const { return paintCells; }
	Math::BSPTree3D* getCollisionTree();
	void clear();
	void clearPaint();
	void loadMapMesh(HRMap::MeshType type, string filename);
	void clearMapMesh(HRMap::MeshType type);
	void generatePaint();

	std::string getMap2DFile() const { return map2DFile; }
	Math::Point getMap2DCenter() const { return map2DCenter; }
	double getMap2DWidth() const { return map2DWidth; }
	double getMap2DHeight() const { return map2DHeight; }
	MapOptions& getMapOptions() { return mapOptions; }

	void setMap2DFile(std::string file) { map2DFile = file; }
	void setMap2DCenter(Math::Point center) { map2DCenter = center; }
	void setMap2DWidth(double width) { map2DWidth = width; }
	void setMap2DHeight(double height) { map2DHeight = height; }

	bool addPropMesh(std::string name, std::string filename);
	bool removePropMesh(int index);
	std::vector<std::string> getPropMeshNames() { return propMeshNames; }
	std::string getPropMeshName(int index);

	const std::vector<OpenGL::Light*>& getLights() const { return lights; }
	void addLight(OpenGL::Light* light);
	void removeLight(OpenGL::Light* light);
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

	Math::BoundingBox3D getMapBoundingBox() const { return mapBoundingBox; }

	std::string getFilename() const { return filename; }

	static std::string meshName(MeshType type);
	static std::string meshTitle(MeshType type);
	static bool meshIsInvisible(MeshType type);
	static bool meshIsSolid(MeshType type);
	static bool meshIsTrack(MeshType type);
	
	void scaleAll(double scale, Math::Point origin = Math::Point());


private:

	void clearCollisionTree();
	void updateDimensions();

};

}  // namespace Map
}  // namespace Project

#endif
