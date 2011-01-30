#include "HRMap.h"
#include "misc/DirectoryFunctions.h"
#include "misc/StdVectorFunctions.h"
#include "paint/PaintGenerator.h"
#include <fstream>
using namespace Project;
using namespace Misc;
using namespace Render;
using namespace Math;
using namespace Paint;
using namespace std;

namespace Project {
namespace Map {

	HRMap::HRMap() {

		cubeMapFile = new CubeMapFile();
		trackRenderable = NULL;
		collisionTree = NULL;
		cubeMap = NULL;

		for (int i = 0; i < NUM_MESHES; i++) {
			mapMesh[i] = NULL;
		}

		clear();
	}

	HRMap::~HRMap() {
		clear();
	}

	bool HRMap::loadMapFile(string _filename) {

		clear();

		string file_directory = DirectoryFunctions::extractDirectory(_filename);

		filename = _filename;

		ifstream in_file;
		in_file.open(filename.c_str());
		while (!in_file.eof()) {

			bool cube_map = false;
			
			string keyword;
			in_file >> keyword;
			if (keyword.length() > 0) {

				if (keyword[0] == '#') {
					//If the line starts with #, ignore the entire line
					in_file.ignore(9999, '\n');
				}
				else if (keyword == "version") {
					in_file >> version;
				}
				else if (keyword == "light") {
					Light* light = new Light();
					char c;
					in_file >> c;
					in_file >> *light;
					lights.push_back(light);
				}
				else  {
					for (int i = 0; i < 6; i++) {
						if (keyword == CubeMapFile::getSideName(i)) {
							string side_file;
							in_file >> side_file;
							cubeMapFile->setSideFile(i, DirectoryFunctions::fromRelativeFilename(file_directory, side_file));
							cube_map = true;
						}
					}

					if (!cube_map) {
						for (int i = 0; i < HRMap::NUM_MESHES; i++) {
							HRMap::MeshType type = static_cast<HRMap::MeshType>(i);
							if (keyword == meshName(type)) {
								in_file >> mapMeshFile[i];
								break;
							}
						}
					}
				}

			}

		}

		//trackRenderable = new RenderList();

		for (int i = 0; i < NUM_MESHES; i++) {
			mapMeshFile[i] = DirectoryFunctions::fromRelativeFilename(file_directory, mapMeshFile[i]);
			MeshType type = static_cast<MeshType>(i);
			loadMapMesh(type, mapMeshFile[i]);
		}

		//cubeMap = new TextureCube(bgImageXPos, bgImageXNeg, bgImageYPos, bgImageYNeg, bgImageZPos, bgImageZNeg);

		return true;

	}

	bool HRMap::saveMapFile(std::string _filename) const {
		return false;
	}

	void HRMap::clearPaint() {
		for (unsigned int i = 0; i < paintCells.size(); i++) {
			delete(paintCells[i]);
		}
		paintCells.clear();
	}

	void HRMap::clear() {

		version = "";
		filename = "";
		cubeMapFile->clear();

		clearCubeMap();
		clearCollisionTree();
		finishPlane = BoundingPlane3D();

		for (int i = 0; i < NUM_MESHES; i++) {
			MeshType type = static_cast<MeshType>(i);
			if (mapMesh[i] != NULL) {
				MeshLoader::getInstance()->deleteModelByName(meshName(type));
				mapMesh[i] = NULL;
				
			}
			mapMeshFile[i] = "";
		}

		for (unsigned int i = 0; i < propMeshNames.size(); i++) {
			MeshLoader::getInstance()->deleteModelByName(propMeshNames[i]);
		}
		propMeshNames.clear();
		propMeshFilenames.clear();

		if (trackRenderable != NULL) {
			delete(trackRenderable);
			trackRenderable = NULL;
		}

		clearLights();
		clearPathNodes();
		clearStartPoints();

		clearPaint();

	}


	void HRMap::loadMapMesh(HRMap::MeshType type, string filename) {
		MeshLoader::getInstance()->deleteModelByName(meshName(type));
		mapMesh[static_cast<int>(type)] = MeshLoader::getInstance()->loadOBJ(meshName(type), filename);
		mapMeshFile[static_cast<int>(type)] = filename;
		if (type != DECOR)
			clearCollisionTree();
	}

	void HRMap::clearMapMesh(HRMap::MeshType type) {
		int i = static_cast<int>(type);
		if (mapMesh[i] != NULL) {
			MeshLoader::getInstance()->deleteModelByName(meshName(type));
			mapMesh[i] = NULL;
			if (type != DECOR) {
				clearCollisionTree();
			}
		}
		mapMeshFile[i] = "";
	}
	void HRMap::setCubeMapFile(const CubeMapFile& file) {
		cubeMapFile->setToFile(file);
		clearCubeMap();
	}
	string HRMap::meshName(MeshType type) {
		switch (type) {
			case HRMap::TRACK:
				return "mapTrackMesh";
			case HRMap::INVIS_TRACK:
				return "mapInvisTrackMesh";
			case HRMap::SOLID:
				return "mapSolidMesh";
			case HRMap::INVIS_SOLID:
				return "mapInvisSolidMesh";
			case HRMap::DECOR:
				return "mapDecor";
			default:
				return "";
		}
	}

	string HRMap::meshTitle(MeshType type) {
		switch (type) {
			case HRMap::TRACK:
				return "Track";
			case HRMap::INVIS_TRACK:
				return "Invisible Track";
			case HRMap::SOLID:
				return "Solid";
			case HRMap::INVIS_SOLID:
				return "Invisible Solid";
			case HRMap::DECOR:
				return "Decor";
			default:
				return "";
		}
	}

	bool HRMap::meshIsInvisible(MeshType type) {
		switch (type) {
			case HRMap::TRACK: case HRMap::SOLID: case HRMap::DECOR:
				return false;
			case HRMap::INVIS_TRACK: case HRMap::INVIS_SOLID:
				return true;
			default:
				return true;
		}
	}

	Render::TextureCube* HRMap::getCubeMap() {
		if (cubeMap == NULL) {
			cubeMap = new TextureCube(*cubeMapFile);
		}
		return cubeMap;
	}
	void HRMap::clearCubeMap() {
		if (cubeMap != NULL) {
			delete(cubeMap);
			cubeMap = NULL;
		}
	}
	void HRMap::generatePaint(double cell_radius) {

		clearPaint();

		vector<Triangle3D> triangles;

		if (getMapMesh(TRACK))
			triangles = getMapMesh(TRACK)->getTriangles();
		if (getMapMesh(INVIS_TRACK))
			vectorAppend(triangles, getMapMesh(INVIS_TRACK)->getTriangles());

		PaintGenerator generator(triangles, cell_radius);
		paintCells = generator.getPaintCells();
	}

	void HRMap::addLight(Light* light) {
		lights.push_back(light);
	}

	void HRMap::removeLight(Light* light) {
		if (Misc::vectorRemoveOneElement(lights, light)) {
			delete(light);
		}
	}

	void HRMap::clearLights() {
		for (unsigned int i = 0; i < lights.size(); i++)
			delete(lights[i]);
		lights.clear();
	}
	void HRMap::addPathNode(PathNode* node) {
		pathNodes.push_back(node);
	}

	void HRMap::removePathNode(PathNode* node) {
		if (Misc::vectorRemoveOneElement(pathNodes, node)) {
			delete(node);
		}
		for (unsigned int i = 0; i < pathNodes.size(); i++) {
			vector<PathNode*>& linked_nodes = pathNodes[i]->getNextNodes();
			vectorRemoveOneElement(linked_nodes, node);
		}
	}
	void HRMap::addStartPoint(Vertex3D* point) {
		startPoints.push_back(point);
	}

	void HRMap::removeStartPoint(Vertex3D* point) {
		if (Misc::vectorRemoveOneElement(startPoints, point)) {
			delete(point);
		}
	}
	void HRMap::clearPathNodes() {
		for (unsigned int i = 0; i < pathNodes.size(); i++)
			delete(pathNodes[i]);
		pathNodes.clear();
	}
	void HRMap::clearStartPoints() {
		for (unsigned int i = 0; i < startPoints.size(); i++)
			delete(startPoints[i]);
		startPoints.clear();
	}
	BSPTree3D* HRMap::getCollisionTree() {

		if (collisionTree == NULL) {

			vector<ObjectSpatial*> collision_triangles;

			BoundingBox3D collision_bound;
			for (int i = 0; i < NUM_MESHES; i++) {
				MeshType type = static_cast<MeshType>(i);
				if (type != DECOR && getMapMesh(type)) {
					vector<Triangle3D> triangles = getMapMesh(type)->getTriangles();
					for (unsigned int t = 0; t < triangles.size(); t++) {
						if (i == 0 && t == 0)
							collision_bound.setToObject(triangles[t]);
						else
							collision_bound.expandToInclude(triangles[t]);
						collision_triangles.push_back(new Triangle3D(triangles[t]));
					}
				}
			}

			collisionTree = new BSPTree3D(collision_bound, BSPTree3D::MIN_OVERLAP);
			collisionTree->add(collision_triangles);
		}

		return collisionTree;

		
	}

	void HRMap::clearCollisionTree() {
		if (collisionTree) {
			vector<ObjectSpatial*> collision_triangles;
			collisionTree->appendAll(&collision_triangles);

			for (unsigned int i = 0; i < collision_triangles.size(); i++)
				delete(collision_triangles[i]);

			delete(collisionTree);
			collisionTree = NULL;
		}
	}

	bool HRMap::addPropMesh(std::string name, std::string filename) {

		if (!vectorContains(propMeshNames, name) && !MeshLoader::getInstance()->getModelByName(name)) {
			if (MeshLoader::getInstance()->loadOBJ(name, filename)) {
				propMeshNames.push_back(name);
				propMeshFilenames.push_back(filename);
				return true;
			}
		}

		return false;

	}
	bool HRMap::removePropMesh(int index) {
		if (index >= 0 && index < propMeshNames.size()) {
			MeshLoader::getInstance()->deleteModelByName(propMeshNames[index]);
			vectorRemoveAtIndex(propMeshNames, index);
			vectorRemoveAtIndex(propMeshFilenames, index);
			return true;
		}
		return false;
	}


}  // namespace Map
}  // namespace Project
