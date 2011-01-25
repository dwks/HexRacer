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
}  // namespace Map
}  // namespace Project
