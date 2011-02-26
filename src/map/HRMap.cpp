#include "HRMap.h"
#include "misc/DirectoryFunctions.h"
#include "misc/StdVectorFunctions.h"
#include "paint/PaintGenerator.h"
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
		delete(cubeMapFile);
	}

	bool HRMap::loadMapFile(string _filename) {

		clear();

		string file_directory = DirectoryFunctions::extractDirectory(_filename);
		filename = _filename;

		vector< vector<int> > pathnode_links;

		ifstream in_file;
		in_file.open(filename.c_str());

		if (!in_file) {
			return false;
		}

		string keyword;
		while (in_file >> keyword) {

			if (keyword[0] == '#') {
				//If the line starts with #, ignore the entire line
				in_file.ignore(9999, '\n');
			}
			else if (keyword == HRMAP_PAINTCELL_LABEL) {
				PaintCell* cell = new PaintCell(Point());
				in_file >> *cell;
				cell->index = paintCells.size();
				paintCells.push_back(cell);
			}
			else if (keyword == HRMAP_PATHNODE_LABEL) {
				Point p;
				int num_links;
				vector<int> links;
				float progress;
				in_file >> p;
				in_file >> progress;
				in_file >> num_links;
				while (num_links > 0) {
					int index;
					in_file >> index;
					links.push_back(index);
					num_links--;
				}
				PathNode* node = new PathNode(p);
				node->index = static_cast<int>(pathNodes.size());
				node->setProgress(progress);
				pathNodes.push_back(node);
				pathnode_links.push_back(links);
			}
			else if (keyword == HRMAP_STARTPOINT_LABEL) {
				Point p;
				in_file >> p;
				startPoints.push_back(new Vertex3D(p));
			}
			else if (keyword == HRMAP_LIGHT_LABEL) {
				Light* light = new Light();
				in_file >> *light;
				lights.push_back(light);
			}
			else if (keyword == HRMAP_MESHINSTANCE_LABEL) {
				MeshInstance* instance = new MeshInstance("", SimpleTransform());
				in_file >> *instance;
				if (!addMeshInstance(instance)) {
					delete(instance);
				}
			}
			else if (keyword == HRMAP_PROPMESH_LABEL) {
				string mesh_name;
				string mesh_filename;
				in_file >> mesh_name;
				in_file >> mesh_filename;
				mesh_filename = DirectoryFunctions::fromRelativeFilename(file_directory, mesh_filename);
				addPropMesh(mesh_name, mesh_filename);
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

		//Convert the mesh filenames from relative to absolute
		for (int i = 0; i < NUM_MESHES; i++) {
			if (mapMeshFile[i].length() > 0) {
				mapMeshFile[i] = DirectoryFunctions::fromRelativeFilename(file_directory, mapMeshFile[i]);
				MeshType type = static_cast<MeshType>(i);
				loadMapMesh(type, mapMeshFile[i]);
			}
		}

		//Link all loaded path nodes
		for (unsigned int i = 0; i < pathnode_links.size(); i++) {
			for (unsigned int j = 0; j < pathnode_links[i].size(); j++) {
				pathNodes[i]->linkToNode(pathNodes[pathnode_links[i][j]]);
				//pathNodes[i]->getNextNodes().push_back(pathNodes[pathnode_links[i][j]]);
			}
		}

		return true;

	}

	bool HRMap::saveMapFile(std::string _filename) {

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

		out_file << "#Prop Meshes\n";
		for (unsigned int i = 0; i < propMeshNames.size(); i++) {
			out_file << HRMAP_PROPMESH_LABEL << ' ' << propMeshNames[i] << ' '
				<< DirectoryFunctions::toRelativeFilename(save_directory, propMeshFilenames[i]) << '\n';
		}

		out_file << "#Finish Plane\n";
		out_file << HRMAP_FINISHPLANE_LABEL << ' ' << finishPlane << '\n';

		out_file << "#Lights\n";
		for (unsigned int i = 0; i < lights.size(); i++) {
			out_file << HRMAP_LIGHT_LABEL << ' ' << (*lights[i]) << '\n';
		}

		out_file << "#Path Nodes\n";

		for (unsigned int i = 0; i < pathNodes.size(); i++)
			pathNodes[i]->index = i;

		for (unsigned int i = 0; i < pathNodes.size(); i++) {

			out_file << HRMAP_PATHNODE_LABEL << ' ' << (*pathNodes[i])
				<< ' ' << pathNodes[i]->getProgress() << ' ';

			const std::vector<PathNode*>& next_nodes = pathNodes[i]->getNextNodes();
			out_file << next_nodes.size() << ' ';
			for (unsigned int i = 0; i < next_nodes.size(); i++) {
				out_file << next_nodes[i]->index;
				if (i < next_nodes.size()-1) {
					out_file << ' ';
				}
			}
			out_file << '\n';
		}

		out_file << "#Start Points\n";
		for (unsigned int i = 0; i < startPoints.size(); i++) {
			out_file << HRMAP_STARTPOINT_LABEL << ' ' << (*startPoints[i]) << '\n';
		}

		out_file << "#Mesh Instances\n";
		for (unsigned int i = 0; i < meshInstances.size(); i++) {
			out_file << HRMAP_MESHINSTANCE_LABEL << ' ' << (*meshInstances[i]) << '\n';
		}

		out_file << "#Paint Cells\n";
		for (unsigned int i = 0; i < paintCells.size(); i++) {
			out_file << HRMAP_PAINTCELL_LABEL << ' ' << (*paintCells[i]) << '\n';
		}


		out_file.close();

		filename = _filename;

		return true;


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
		clearMeshInstances();

		clearPaint();

	}


	void HRMap::loadMapMesh(HRMap::MeshType type, string filename) {
		MeshLoader::getInstance()->deleteModelByName(meshName(type), true);
		mapMesh[static_cast<int>(type)] = MeshLoader::getInstance()->loadOBJ(meshName(type), filename, true);
		mapMeshFile[static_cast<int>(type)] = filename;
		if (meshIsSolid(type))
			clearCollisionTree();
		updateMapBoundingBox();
	}

	void HRMap::clearMapMesh(HRMap::MeshType type) {
		int i = static_cast<int>(type);
		if (mapMesh[i] != NULL) {
			MeshLoader::getInstance()->deleteModelByName(meshName(type));
			mapMesh[i] = NULL;
			if (meshIsSolid(type)) {
				clearCollisionTree();
			}
			updateMapBoundingBox();
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

		//Find all nodes with a link to or from the node to delete
		vector<PathNode*> linked_nodes;
		linked_nodes.insert(linked_nodes.end(), node->getNextNodes().begin(), node->getNextNodes().end());
		linked_nodes.insert(linked_nodes.end(), node->getPreviousNodes().begin(), node->getPreviousNodes().end());

		//Disassociate all linked nodes with the deleted node
		for (unsigned int i = 0; i < linked_nodes.size(); i++)
			linked_nodes[i]->disassociateNode(node);

		//Delete the node
		if (Misc::vectorRemoveOneElement(pathNodes, node))
			delete(node);
		
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
	bool HRMap::addMeshInstance(MeshInstance* mesh) {
		if (vectorContains(propMeshNames, mesh->getMeshName())) {
			meshInstances.push_back(mesh);
			return true;
		}
		else return false;
	}
	void HRMap::removeMeshInstance(MeshInstance* mesh) {
		if (Misc::vectorRemoveOneElement(meshInstances, mesh)) {
			delete(mesh);
		}
	}
	void HRMap::clearMeshInstances() {
		for (unsigned int i = 0; i < meshInstances.size(); i++)
			delete(meshInstances[i]);
		meshInstances.clear();
	}
	BSPTree3D* HRMap::getCollisionTree() {

		if (collisionTree == NULL) {

			vector<ObjectSpatial*> collision_triangles;

			BoundingBox3D collision_bound;
			for (int i = 0; i < NUM_MESHES; i++) {
				MeshType type = static_cast<MeshType>(i);
				if (meshIsSolid(type) && getMapMesh(type)) {
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
			collisionTree->appendAll(collision_triangles);

			for (unsigned int i = 0; i < collision_triangles.size(); i++)
				delete(collision_triangles[i]);

			delete(collisionTree);
			collisionTree = NULL;
		}
	}

	bool HRMap::addPropMesh(std::string name, std::string filename) {

		if (!vectorContains(propMeshNames, name) && !MeshLoader::getInstance()->getModelByName(name, true)) {
			if (MeshLoader::getInstance()->loadOBJ(name, filename)) {
				propMeshNames.push_back(name);
				propMeshFilenames.push_back(filename);
				return true;
			}
		}

		return false;

	}
	bool HRMap::removePropMesh(int index) {
		if (index >= 0 && index < static_cast<int>(propMeshNames.size())) {
			MeshLoader::getInstance()->deleteModelByName(propMeshNames[index]);
			for (unsigned int i = 0; i < meshInstances.size(); i++) {
				if (meshInstances[i]->getMeshName() == propMeshNames[index]) {
					removeMeshInstance(meshInstances[i]);
				}
			}
			vectorRemoveAtIndex(propMeshNames, index);
			vectorRemoveAtIndex(propMeshFilenames, index);
			return true;
		}
		return false;
	}


	std::string HRMap::getPropMeshName(int index) {
		if (index >= 0 && index < static_cast<int>(propMeshNames.size()))
			return propMeshNames[index];
		return "";
	}

	void HRMap::updateMapBoundingBox() {

		bool set = false;

		for (int i = 0; i < NUM_MESHES; i++) {
			MeshType type = static_cast<MeshType>(i);
			if (getMapMesh(type)) {
				if (!set) {
					mapBoundingBox.setToObject(getMapMesh(type)->getBoundingBox());
					set = true;
				}
				else {
					mapBoundingBox.expandToInclude(getMapMesh(type)->getBoundingBox());
				}
			}
		}

	}

	void HRMap::scaleAll(double scale, Point origin) {
		for (unsigned int i = 0; i < lights.size(); i++) {
			lights[i]->translate(-origin);
			lights[i]->moveCentroid(lights[i]->getPosition()*scale);
			lights[i]->translate(origin);
		}
		for (unsigned int i = 0; i < pathNodes.size(); i++) {
			pathNodes[i]->translate(-origin);
			pathNodes[i]->moveCentroid(pathNodes[i]->getPosition()*scale);
			pathNodes[i]->translate(origin);
		}
		for (unsigned int i = 0; i < startPoints.size(); i++) {
			startPoints[i]->translate(-origin);
			startPoints[i]->moveCentroid(startPoints[i]->getPosition()*scale);
			startPoints[i]->translate(origin);
		}
		for (unsigned int i = 0; i < meshInstances.size(); i++) {
			SimpleTransform transform = meshInstances[i]->getTransformation();
			transform.setScale(transform.getScale()*scale);
			transform.translate(-origin);
			transform.setTranslation(transform.getTranslation()*scale);
			transform.translate(origin);
			meshInstances[i]->setTransformation(transform);
		}
		finishPlane.translate(-origin);
		finishPlane.moveCentroid(finishPlane.centroid()*scale);
		finishPlane.translate(origin);
	}

}  // namespace Map
}  // namespace Project
