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

		trackRenderable = NULL;
		cubeMap = NULL;
		collisionTree = NULL;

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
				else if (keyword == "bgimgxpos") {
					in_file >> bgImageXPos;
				}
				else if (keyword == "bgimgxneg") {
					in_file >> bgImageXNeg;
				}
				else if (keyword == "bgimgypos") {
					in_file >> bgImageYPos;
				}
				else if (keyword == "bgimgyneg") {
					in_file >> bgImageYNeg;
				}
				else if (keyword == "bgimgzpos") {
					in_file >> bgImageZPos;
				}
				else if (keyword == "bgimgzneg") {
					in_file >> bgImageZNeg;
				}
				else if (keyword == "light") {
					Light* light = new Light();
					char c;
					in_file >> c;
					in_file >> *light;
					lights.push_back(light);
				}
				else {
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

		bgImageXPos = DirectoryFunctions::fromRelativeFilename(file_directory, bgImageXPos);
		bgImageXNeg = DirectoryFunctions::fromRelativeFilename(file_directory, bgImageXNeg);
		bgImageYPos = DirectoryFunctions::fromRelativeFilename(file_directory, bgImageYPos);
		bgImageYNeg = DirectoryFunctions::fromRelativeFilename(file_directory, bgImageYNeg);
		bgImageZPos = DirectoryFunctions::fromRelativeFilename(file_directory, bgImageZPos);
		bgImageZNeg = DirectoryFunctions::fromRelativeFilename(file_directory, bgImageZNeg);
		//trackRenderable = new RenderList();

		for (int i = 0; i < NUM_MESHES; i++) {
			mapMeshFile[i] = DirectoryFunctions::fromRelativeFilename(file_directory, mapMeshFile[i]);
			MeshType type = static_cast<MeshType>(i);
			loadMapMesh(type, mapMeshFile[i]);
		}

		cubeMap = new TextureCube(bgImageXPos, bgImageXNeg, bgImageYPos, bgImageYNeg, bgImageZPos, bgImageZNeg);

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
		bgImageXPos = "";
		bgImageXNeg = "";
		bgImageYPos = "";
		bgImageYNeg = "";
		bgImageZPos = "";
		bgImageZNeg = "";

		clearCollisionTree();

		for (int i = 0; i < NUM_MESHES; i++) {
			MeshType type = static_cast<MeshType>(i);
			if (mapMesh[i] != NULL) {
				MeshLoader::getInstance()->deleteModelByName(meshName(type));
				mapMesh[i] = NULL;
				
			}
			mapMeshFile[i] = "";
		}

		if (trackRenderable != NULL) {
			delete(trackRenderable);
			trackRenderable = NULL;
		}

		if (cubeMap != NULL) {
			delete(cubeMap);
			cubeMap = NULL;
		}

		for (unsigned int i = 0; i < lights.size(); i++) {
			delete(lights[i]);
		}

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
							collision_bound.setToObject(triangles[i]);
						else
							collision_bound.expandToInclude(triangles[i]);
						collision_triangles.push_back(new Triangle3D(triangles[i]));
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

}  // namespace Map
}  // namespace Project
