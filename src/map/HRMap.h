#ifndef PROJECT_MAP__MAP_FILE_H
#define PROJECT_MAP__MAP_FILE_H

//#include "mesh/MeshLoader.h"
#include "opengl/TextureCube.h"
#include "opengl/CubeMapFile.h"
//#include "opengl/Light.h"
#include "mesh/MeshGroup.h"
#include "paint/PaintCell.h"
#include "paint/PaintCellInfo.h"
#include "math/BoundingPlane3D.h"
#include "math/HexGrid.h"
#include "math/HexHeightMap.h"
#include "misc/ProgressTracker.h"
#include "MapOptions.h"
#include "HRMapObjects.h"
#include <string>
#include <vector>

#define HRMAP_NUM_MESHES 5
#define HRMAP_VERSION "0.2.1"
#define HRMAP_VERSION_LABEL "version"
#define HRMAP_FINISHPLANE_LABEL "finishPlane"
#define HRMAP_MAP2DFILE_LABEL "map2DFile"
#define HRMAP_MAP2DCENTER_LABEL "map2DCenter"
#define HRMAP_MAP2DWIDTH_LABEL "map2DWidth"
#define HRMAP_MAP2DHEIGHT_LABEL "map2DHeight"
#define HRMAP_HEXGRID_LABEL "hexGrid"
#define HRMAP_PAINTHEIGHTMAP_LABEL "paintHeightMap"

namespace Project {
namespace Map {

class HRMap {
private:

	Mesh::MeshGroup* mapMesh[HRMAP_NUM_MESHES];
	std::string mapMeshFile[HRMAP_NUM_MESHES];

	std::string version;
	std::string filename;
	OpenGL::CubeMapFile* cubeMapFile;
	OpenGL::TextureCube* cubeMap;

	Math::BoundingPlane3D finishPlane;
	Math::BoundingBox3D mapBoundingBox;

	HRMapObjects mapObjects;

	std::string map2DFile;
	Math::Point map2DCenter;
	double map2DWidth;
	double map2DHeight;

	MapOptions mapOptions;

	Math::BSPTree3D* collisionTree;
	Math::HexGrid hexGrid;
	Math::HexHeightMap paintHeightMap;

	std::vector<Paint::PaintCellInfo> paintCellInfo;

public:

	enum MeshType {TRACK, INVIS_TRACK, SOLID, INVIS_SOLID, DECOR, NUM_MESHES};

	HRMap();
	~HRMap();
	bool loadMapFile(std::string _filename, Misc::ProgressTracker* progress_tracker = NULL);
	bool saveMapFile(std::string _filename, Misc::ProgressTracker* progress_tracker = NULL);
	Mesh::MeshGroup* getMapMesh(MeshType type) const { return mapMesh[static_cast<int>(type)]; }
	OpenGL::TextureCube* getCubeMap();
	const OpenGL::CubeMapFile* getCubeMapFile() const { return cubeMapFile; }
	void setCubeMapFile(const OpenGL::CubeMapFile& file);
	void clearCubeMap();
	Math::BSPTree3D* getCollisionTree();
	void clear();
	void clearPaint();
	void loadMapMesh(HRMap::MeshType type, string filename);
	void clearMapMesh(HRMap::MeshType type);
	void generatePaint(Misc::ProgressTracker* progress_tracker = NULL);

	const Math::HexGrid& getHexGrid() const { return hexGrid; }
	const Math::HexHeightMap& getPaintHeightMap() const { return paintHeightMap; }

	const vector<Paint::PaintCellInfo>& getPaintCellInfo() const { return paintCellInfo; }

	std::string getMap2DFile() const { return map2DFile; }
	Math::Point getMap2DCenter() const { return map2DCenter; }
	double getMap2DWidth() const { return map2DWidth; }
	double getMap2DHeight() const { return map2DHeight; }
	MapOptions& getMapOptions() { return mapOptions; }

	void setMap2DFile(std::string file) { map2DFile = file; }
	void setMap2DCenter(Math::Point center) { map2DCenter = center; }
	void setMap2DWidth(double width) { map2DWidth = width; }
	void setMap2DHeight(double height) { map2DHeight = height; }

	HRMapObjects& getMapObjects() { return mapObjects; }

	Math::BoundingPlane3D& getFinishPlane() { return finishPlane; }

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
	void updateHexGrid();
	
	void loadHeightMap(std::ifstream& stream, Math::HexHeightMap& height_map, Misc::ProgressTracker* progress_tracker = NULL);
	void saveHeightMap(std::ofstream& stream, Math::HexHeightMap& height_map, Misc::ProgressTracker* progress_tracker = NULL);

};

}  // namespace Map
}  // namespace Project

#endif
