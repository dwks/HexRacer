#include "HRMap.h"
#include "misc/DirectoryFunctions.h"
#include "misc/StdVectorFunctions.h"
#include "paint/PaintGenerator.h"
#include "mesh/MeshLoader.h"
#include <fstream>
using namespace Project;
using namespace Misc;
using namespace Render;
using namespace Mesh;
using namespace Math;
using namespace Paint;
using namespace OpenGL;
using namespace std;

namespace Project {
namespace Map {

	HRMap::HRMap()
		:hexGrid(PAINT_CELL_RADIUS, 0.0, 1.0, 0.0, 1.0) {

		cubeMapFile = new CubeMapFile();
		collisionTree = NULL;
		cubeMap = NULL;

		for (int i = 0; i < NUM_MESHES; i++) {
			mapMesh[i] = NULL;
		}

		clear();
	}

	HRMap::~HRMap() {
		clear();
		delete cubeMapFile;
	}

	bool HRMap::loadMapFile(string _filename, Misc::ProgressTracker* progress_tracker) {

		clear();

		string file_directory = DirectoryFunctions::extractDirectory(_filename);
		filename = _filename;

		ifstream in_file;
		in_file.open(filename.c_str());

		if (!in_file)
			return false;

		int file_size;
		in_file.seekg(0, ios_base::end);
		file_size = in_file.tellg();
		in_file.seekg(0, ios_base::beg);

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Loading map file...");
			progress_tracker->setTotalSteps(100);
			progress_tracker->setCurrentStep(0);
		}

		mapObjects.parseBegin();

		string keyword;
		while (in_file >> keyword) {

			if (progress_tracker) {
				float progress = (float) in_file.tellg() / (float) file_size;
				progress_tracker->setCurrentStep(static_cast<int>(progress*50));
			}

			if (keyword[0] == '#') {
				//If the line starts with #, ignore the entire line
				in_file.ignore(9999, '\n');
			}
			else if (keyword == HRMAP_PAINTCELL_LABEL) {

				if (version >= "0.2.0") {
					Point normal;
					int num_cells;
					in_file >> normal >> num_cells;
					while (num_cells > 0) {
						Paint::PaintCellInfo cell_info(0, 0);
						in_file >> cell_info;
						cell_info.normal = normal;
						cell_info.normal.postNormalize();
						paintCellInfo.push_back(cell_info);
						num_cells--;
					}
				}

			}
			else if (mapObjects.parseStream(keyword, in_file, file_directory, version)) {
			}
			else if (keyword == HRMAP_HEXGRID_LABEL) {
				in_file >> hexGrid;
				paintHeightMap.setHexGrid(hexGrid);
			}
			else if (keyword == HRMAP_PAINTHEIGHTMAP_LABEL) {
				if (version <= "0.2.0") {
					in_file >> paintHeightMap;
				}
				else {
					loadHeightMap(in_file, paintHeightMap, progress_tracker);
					if (progress_tracker) {
						progress_tracker->setCurrentStage("Loading map file...");
						progress_tracker->setTotalSteps(100);
					}
				}
			}
			else if (keyword == HRMAP_FINISHPLANE_LABEL) {
				in_file >> finishPlane;
			}
			else if (keyword == HRMAP_MAP2DFILE_LABEL) {
				in_file >> map2DFile;
				map2DFile = DirectoryFunctions::fromRelativeFilename(file_directory, map2DFile);
			}
			else if (keyword == HRMAP_MAP2DCENTER_LABEL) {
				in_file >> map2DCenter;
			}
			else if (keyword == HRMAP_MAP2DWIDTH_LABEL) {
				in_file >> map2DWidth;
			}
			else if (keyword == HRMAP_MAP2DHEIGHT_LABEL) {
				in_file >> map2DHeight;
			}
			else if (keyword == HRMAP_VERSION_LABEL) {
				in_file >> version;
			}
			else if (mapOptions.parseStream(keyword, in_file, version)) {
			}
			else  {

				bool cube_map = false;

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
		in_file.close();

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Loading map meshes...");
		}

		//Convert the mesh filenames from relative to absolute
		for (int i = 0; i < NUM_MESHES; i++) {
			if (mapMeshFile[i].length() > 0) {
				mapMeshFile[i] = DirectoryFunctions::fromRelativeFilename(file_directory, mapMeshFile[i]);
				MeshType type = static_cast<MeshType>(i);
				loadMapMesh(type, mapMeshFile[i]);
			}
			if (progress_tracker)
				progress_tracker->setCurrentStep(50+i*(50/NUM_MESHES));
		}

		mapObjects.parseFinish();

		return true;

	}

	bool HRMap::saveMapFile(std::string _filename, Misc::ProgressTracker* progress_tracker) {

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Saving map file...");
			progress_tracker->setTotalSteps(mapObjects.getNumObjects()
				+paintHeightMap.getSetHexIndices().size()
				+paintHeightMap.getSetVertexIndices().size()
				+paintCellInfo.size());
			progress_tracker->setCurrentStep(0);
		}

		string save_directory = DirectoryFunctions::extractDirectory(_filename);

		ofstream out_file;
		out_file.open(_filename.c_str());
		out_file << "#HR Map File\n";
		out_file << "version " << HRMAP_VERSION << '\n';

		out_file << "#Map Meshes\n";
		for (int i = 0; i < NUM_MESHES; i++) {
			if (mapMeshFile[i].length() > 0) {
				out_file << meshName(static_cast<MeshType>(i)) << ' '
					<< DirectoryFunctions::toRelativeFilename(save_directory, mapMeshFile[i])
						<< '\n';
			}
		}

		out_file << "#Background Images\n";
		for (int i = 0; i < 6; i++) {
			if (cubeMapFile->getSideFile(i).length() > 0) {
				out_file << cubeMapFile->getSideName(i) << ' '
					<< DirectoryFunctions::toRelativeFilename(save_directory, cubeMapFile->getSideFile(i))
					<< '\n';
			}
		}

		out_file << "#2D Map\n";
		if (map2DFile.length() > 0) {
			out_file << HRMAP_MAP2DFILE_LABEL << ' '
				<< DirectoryFunctions::toRelativeFilename(save_directory, map2DFile) << '\n';
			out_file << HRMAP_MAP2DCENTER_LABEL << ' ' << map2DCenter << '\n';
			out_file << HRMAP_MAP2DWIDTH_LABEL << ' ' << map2DWidth << '\n';
			out_file << HRMAP_MAP2DHEIGHT_LABEL << ' ' << map2DHeight << '\n';
		}

		out_file << "#Finish Plane\n";
		out_file << HRMAP_FINISHPLANE_LABEL << ' ' << finishPlane << '\n';

		mapOptions.saveToStream(out_file);

		mapObjects.saveToStream(out_file, save_directory, progress_tracker);

		out_file << "#HexGrid\n";
		out_file << HRMAP_HEXGRID_LABEL << ' ' << hexGrid << '\n';
		out_file << "#Paint Height Map\n";
		//out_file << HRMAP_PAINTHEIGHTMAP_LABEL << ' ' << paintHeightMap << '\n';
		out_file << HRMAP_PAINTHEIGHTMAP_LABEL << '\n';
		saveHeightMap(out_file, paintHeightMap, progress_tracker);

		if (progress_tracker)
			progress_tracker->setCurrentStage("Saving Paint Cells...");

		out_file << "#Paint Cells\n";

		if (!Misc::vectorIsSorted(paintCellInfo))
			Misc::vectorMergeSort(paintCellInfo, 0, paintCellInfo.size()-1);

		unsigned int i = 0;
		while (i < paintCellInfo.size()) {

			Math::Point last_normal = paintCellInfo[i].normal;

			unsigned int num_same = 1;
			while (i+num_same < paintCellInfo.size()
				&& last_normal == paintCellInfo[i+num_same].normal) {
				num_same++;
			}

			out_file << HRMAP_PAINTCELL_LABEL << ' ' << last_normal << ' ' << num_same << '\n';

			while (num_same > 0) {

				if (progress_tracker)
					progress_tracker->incrementStep();

				out_file << paintCellInfo[i] << '\n';
				i++;
				num_same--;
			}

		}

		out_file.close();

		filename = _filename;

		return true;


	}

	void HRMap::clearPaint() {
		paintCellInfo.clear();
		paintHeightMap.clear();
	}

	void HRMap::clear() {

		version = "";
		filename = "";
		cubeMapFile->clear();

		mapOptions.clear();
		mapObjects.clear();

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
		
		clearPaint();

	}


	void HRMap::loadMapMesh(HRMap::MeshType type, string filename) {
		MeshLoader::getInstance()->deleteModelByName(meshName(type), true);
		mapMesh[static_cast<int>(type)] = MeshLoader::getInstance()->loadOBJ(meshName(type), filename, true);
		mapMeshFile[static_cast<int>(type)] = filename;
		if (meshIsSolid(type))
			clearCollisionTree();

		updateDimensions();
	}

	void HRMap::clearMapMesh(HRMap::MeshType type) {
		int i = static_cast<int>(type);
		if (mapMesh[i] != NULL) {
			MeshLoader::getInstance()->deleteModelByName(meshName(type));
			mapMesh[i] = NULL;
			if (meshIsSolid(type)) {
				clearCollisionTree();
			}
			updateDimensions();
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

	bool HRMap::meshIsSolid(MeshType type) {
		switch (type) {
			case HRMap::DECOR:
				return false;
			default:
				return true;
		}
	}
	bool HRMap::meshIsTrack(MeshType type) {
		switch (type) {
			case HRMap::TRACK: case HRMap::INVIS_TRACK:
				return true;
			default:
				return false;
		}
	}
	OpenGL::TextureCube* HRMap::getCubeMap() {
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
	void HRMap::generatePaint(Misc::ProgressTracker* progress_tracker) {

		clearPaint();

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Collecting map triangles...");
			progress_tracker->setTotalSteps(NUM_MESHES);
			progress_tracker->setCurrentStep(0);
		}

		vector<Triangle3D> triangles;

		for (int i = 0; i < NUM_MESHES; i++) {
			progress_tracker->setCurrentStep(i);
			MeshType type = static_cast<MeshType>(i);
			if (meshIsTrack(type) && getMapMesh(type)) {
				getMapMesh(type)->appendTriangles(triangles);
			}
		}

		updateHexGrid();
		paintHeightMap.setHexGrid(hexGrid);

		PaintGenerator generator;
		generator.generateHeightmap(triangles, paintHeightMap, progress_tracker);
		generator.generateCells(paintHeightMap, paintCellInfo, progress_tracker);

		//paintHeightMap = generator.getHeightMap();
		//paintCellInfo = generator.getCellInfo();
	}	
	BSPTree3D* HRMap::getCollisionTree() {

		if (collisionTree == NULL) {

			vector<ObjectSpatial*> collision_triangles;

			BoundingBox3D collision_bound;
			bool collision_bound_set = false;

			for (int i = 0; i < NUM_MESHES; i++) {
				MeshType type = static_cast<MeshType>(i);
				if (meshIsSolid(type) && getMapMesh(type)) {
					if (!collision_bound_set)
						collision_bound.setToObject(getMapMesh(type)->getBoundingBox());
					else
						collision_bound.expandToInclude(getMapMesh(type)->getBoundingBox());

					vector<Triangle3D> triangles;
					getMapMesh(type)->appendTriangles(triangles);
					for (unsigned int t = 0; t < triangles.size(); t++) {
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
			collisionTree->appendAll(collision_triangles);

			for (unsigned int i = 0; i < collision_triangles.size(); i++)
				delete(collision_triangles[i]);

			delete(collisionTree);
			collisionTree = NULL;
		}
	}

	void HRMap::updateDimensions() {

		bool box_set = false;

		for (int i = 0; i < NUM_MESHES; i++) {

			MeshType type = static_cast<MeshType>(i);
			if (getMapMesh(type)) {

				if (!box_set) {
					mapBoundingBox.setToObject(getMapMesh(type)->getBoundingBox());
					box_set = true;
				}
				else {
					mapBoundingBox.expandToInclude(getMapMesh(type)->getBoundingBox());
				}
			}
		}
	}

	void HRMap::updateHexGrid() {

		bool hexgrid_set = false;
		double min_track_x = 0.0;
		double max_track_x = 0.0;
		double min_track_z = 0.0;
		double max_track_z = 0.0;

		hexGrid.setHexRadius(PAINT_CELL_RADIUS);

		for (int i = 0; i < NUM_MESHES; i++) {

			MeshType type = static_cast<MeshType>(i);
			if (getMapMesh(type)) {
				if (meshIsTrack(type)) {
					if (!hexgrid_set) {
						min_track_x = getMapMesh(type)->getBoundingBox().minX();
						max_track_x = getMapMesh(type)->getBoundingBox().maxX();
						min_track_z = getMapMesh(type)->getBoundingBox().minZ();
						max_track_z = getMapMesh(type)->getBoundingBox().maxZ();
						hexgrid_set = true;
					}
					else {
						min_track_x = Math::minimum(min_track_x, getMapMesh(type)->getBoundingBox().minX());
						max_track_x = Math::maximum(max_track_x, getMapMesh(type)->getBoundingBox().maxX());
						min_track_z = Math::minimum(min_track_z, getMapMesh(type)->getBoundingBox().minZ());
						max_track_z = Math::maximum(max_track_z, getMapMesh(type)->getBoundingBox().maxZ());
					}
				}
			}

		}

		hexGrid.setDimensions(min_track_x, max_track_x, min_track_z, max_track_z);

	}

	void HRMap::scaleAll(double scale, Point origin) {
		
		mapObjects.scaleAll(scale, origin);

		finishPlane.translate(-origin);
		finishPlane.moveCentroid(finishPlane.centroid()*scale);
		finishPlane.translate(origin);
	}

	void HRMap::translateAll(Point translation) {
		mapObjects.translateAll(translation);
		finishPlane.translate(translation);
	}
	void HRMap::loadHeightMap(std::ifstream& stream, Math::HexHeightMap& height_map, Misc::ProgressTracker* progress_tracker) {

		int num_hexes;
		int num_verts;

		stream >> num_hexes >> num_verts;

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Loading paint heightmap..");
			progress_tracker->setTotalSteps(num_hexes+num_verts);
			progress_tracker->setCurrentStep(0);
		}

		while (num_hexes > 0) {

			HexGrid::HexIndex index;
			int row_size;

			stream >> index >> row_size;

			while (row_size > 0) {

				if (progress_tracker)
					progress_tracker->incrementStep();

				int heights;
				stream >> heights;
				
				while (heights > 0) {
					double height;
					stream >> height;
					height_map.addHexHeight(index, height);
					heights--;
				}
				
				index.uIndex++;

				row_size--;
				num_hexes--;
			}
			
		}

		while (num_verts > 0) {

			HexGrid::HexIndex index;
			int row_size;

			stream >> index >> row_size;

			while (row_size > 0) {

				if (progress_tracker)
					progress_tracker->incrementStep();

				int heights;
				stream >> heights;
				
				while (heights > 0) {
					double height;
					stream >> height;
					height_map.addVertexHeight(index, height);
					heights--;
				}
				
				index.uIndex++;

				row_size--;
				num_verts--;
			}
			
		}

		

	}
	void HRMap::saveHeightMap(std::ofstream& stream, Math::HexHeightMap& height_map, Misc::ProgressTracker* progress_tracker) {

		if (progress_tracker) {
			progress_tracker->setCurrentStage("Saving height map...");
		}

		const std::vector<HexGrid::HexIndex>& set_hex_indices = height_map.getSetHexIndices();
		const std::vector<HexGrid::HexIndex>& set_vert_indices = height_map.getSetVertexIndices();
		stream << set_hex_indices.size() << '\n';
		stream << set_vert_indices.size() << '\n';

		int current_index = 0;

		while (current_index < static_cast<int>(set_hex_indices.size())) {
			int row_size = 1;
			HexGrid::HexIndex index = set_hex_indices[current_index];
			while (current_index+row_size < static_cast<int>(set_hex_indices.size()) &&
				set_hex_indices[current_index+row_size].uIndex == index.uIndex+1 &&
				set_hex_indices[current_index+row_size].vIndex == index.vIndex) {
				row_size++;
				index.uIndex++;
			}

			stream << set_hex_indices[current_index] << ' ' << row_size << '\n';

			while (row_size > 0) {

				if (progress_tracker) {
					progress_tracker->incrementStep();
				}

				const std::list<double>* list = height_map.getHexHeights(set_hex_indices[current_index]);
				stream << list->size() << ' ';
				for (std::list<double>::const_iterator it = list->begin(); it != list->end(); ++it) {
					stream << *it << ' ';
				}
				stream << '\n';
				row_size--;
				current_index++;
			}
		}

		current_index = 0;

		while (current_index < static_cast<int>(set_vert_indices.size())) {
			int row_size = 1;
			HexGrid::HexIndex index = set_vert_indices[current_index];
			while (current_index+row_size < static_cast<int>(set_vert_indices.size()) &&
				set_vert_indices[current_index+row_size].uIndex == index.uIndex+1 &&
				set_vert_indices[current_index+row_size].vIndex == index.vIndex) {
				row_size++;
				index.uIndex++;
			}

			stream << set_vert_indices[current_index] << ' ' << row_size << '\n';

			while (row_size > 0) {

				if (progress_tracker) {
					progress_tracker->incrementStep();
				}

				const std::list<double>* list = height_map.getVertexHeights(set_vert_indices[current_index]);
				stream << list->size() << ' ';
				for (std::list<double>::const_iterator it = list->begin(); it != list->end(); ++it) {
					stream << *it << ' ';
				}
				stream << '\n';
				row_size--;
				current_index++;
			}
		}

	}
}  // namespace Map
}  // namespace Project
