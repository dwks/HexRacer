#include "HRMapObjects.h"
#include "misc/StdVectorFunctions.h"
#include "misc/DirectoryFunctions.h"
#include "mesh/MeshLoader.h"

namespace Project {
namespace Map {

	void HRMapObjects::addLight(OpenGL::Light* light) {
		lights.push_back(light);
	}

	void HRMapObjects::removeLight(OpenGL::Light* light) {
		if (Misc::vectorRemoveOneElement(lights, light)) {
			delete(light);
		}
	}
	void HRMapObjects::clear() {

		Mesh::MeshLoader* loader = Mesh::MeshLoader::getInstance();
		for (unsigned int i = 0; i < propMeshNames.size(); i++) {
			loader->deleteModelByName(propMeshNames[i]);
		}

		propMeshNames.clear();
		propMeshFilenames.clear();

		clearLights();
		clearPathNodes();
		clearStartPoints();
		clearMeshInstances();
	}

	void HRMapObjects::clearLights() {
		for (unsigned int i = 0; i < lights.size(); i++)
			delete(lights[i]);
		lights.clear();
	}
	void HRMapObjects::addPathNode(PathNode* node) {
		pathNodes.push_back(node);
	}

	void HRMapObjects::removePathNode(PathNode* node) {

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
	void HRMapObjects::addStartPoint(Math::Vertex3D* point) {
		startPoints.push_back(point);
	}

	void HRMapObjects::removeStartPoint(Math::Vertex3D* point) {
		if (Misc::vectorRemoveOneElement(startPoints, point)) {
			delete(point);
		}
	}
	void HRMapObjects::clearPathNodes() {
		for (unsigned int i = 0; i < pathNodes.size(); i++)
			delete(pathNodes[i]);
		pathNodes.clear();
	}
	void HRMapObjects::clearStartPoints() {
		for (unsigned int i = 0; i < startPoints.size(); i++)
			delete(startPoints[i]);
		startPoints.clear();
	}
	bool HRMapObjects::addMeshInstance(MeshInstance* mesh) {
		if (Misc::vectorContains(propMeshNames, mesh->getMeshName())) {
			meshInstances.push_back(mesh);
			return true;
		}
		else return false;
	}
	void HRMapObjects::removeMeshInstance(MeshInstance* mesh) {
		if (Misc::vectorRemoveOneElement(meshInstances, mesh)) {
			delete(mesh);
		}
	}
	void HRMapObjects::clearMeshInstances() {
		for (unsigned int i = 0; i < meshInstances.size(); i++)
			delete(meshInstances[i]);
		meshInstances.clear();
	}

	bool HRMapObjects::addPropMesh(std::string name, std::string filename) {

		if (!Misc::vectorContains(propMeshNames, name) && !Mesh::MeshLoader::getInstance()->getModelByName(name, true)) {
			if (Mesh::MeshLoader::getInstance()->loadOBJ(name, filename)) {
				propMeshNames.push_back(name);
				propMeshFilenames.push_back(filename);
				return true;
			}
		}

		return false;

	}
	bool HRMapObjects::removePropMesh(int index) {
		if (index >= 0 && index < static_cast<int>(propMeshNames.size())) {
			Mesh::MeshLoader::getInstance()->deleteModelByName(propMeshNames[index]);
			for (unsigned int i = 0; i < meshInstances.size(); i++) {
				if (meshInstances[i]->getMeshName() == propMeshNames[index]) {
					removeMeshInstance(meshInstances[i]);
				}
			}
			Misc::vectorRemoveAtIndex(propMeshNames, index);
			Misc::vectorRemoveAtIndex(propMeshFilenames, index);
			return true;
		}
		return false;
	}


	std::string HRMapObjects::getPropMeshName(int index) {
		if (index >= 0 && index < static_cast<int>(propMeshNames.size()))
			return propMeshNames[index];
		return "";
	}
	void HRMapObjects::scaleAll(double scale, Math::Point origin) {

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
			Math::SimpleTransform transform = meshInstances[i]->getTransformation();
			transform.setScale(transform.getScale()*scale);
			transform.translate(-origin);
			transform.setTranslation(transform.getTranslation()*scale);
			transform.translate(origin);
			meshInstances[i]->setTransformation(transform);
		}

	}

	void HRMapObjects::translateAll(Math::Point translation) {

		for (unsigned int i = 0; i < lights.size(); i++) {
			lights[i]->translate(translation);
		}
		for (unsigned int i = 0; i < pathNodes.size(); i++) {
			pathNodes[i]->translate(translation);
		}
		for (unsigned int i = 0; i < startPoints.size(); i++) {
			startPoints[i]->translate(translation);
		}
		for (unsigned int i = 0; i < meshInstances.size(); i++) {
			Math::SimpleTransform transform = meshInstances[i]->getTransformation();
			transform.translate(translation);
			meshInstances[i]->setTransformation(transform);
		}

	}
	bool HRMapObjects::parseStream(const std::string& keyword, std::ifstream& stream,
		const std::string& load_directory, const std::string& version) {

		if (keyword == HRMAP_PATHNODE_LABEL) {
			Math::Point p;
			int num_links;
			vector<int> links;
			float progress;
			stream >> p;
			stream >> progress;
			stream >> num_links;
			while (num_links > 0) {
				int index;
				stream >> index;
				links.push_back(index);
				num_links--;
			}
			PathNode* node = new PathNode(p);
			node->index = static_cast<int>(pathNodes.size());
			node->setProgress(progress);
			pathNodes.push_back(node);
			pathnodeLinks.push_back(links);
			return true;
		}
		else if (keyword == HRMAP_STARTPOINT_LABEL) {
			Math::Point p;
			stream >> p;
			startPoints.push_back(new Math::Vertex3D(p));
			return true;
		}
		else if (keyword == HRMAP_LIGHT_LABEL) {
			OpenGL::Light* light = new OpenGL::Light();
			stream >> *light;
			lights.push_back(light);
			return true;
		}
		else if (keyword == HRMAP_MESHINSTANCE_LABEL) {
			MeshInstance* instance = new MeshInstance("", Math::SimpleTransform());
			stream >> *instance;
			if (!addMeshInstance(instance)) {
				delete(instance);
			}
			return true;
		}
		else if (keyword == HRMAP_PROPMESH_LABEL) {
			string mesh_name;
			string mesh_filename;
			stream >> mesh_name;
			stream >> mesh_filename;
			mesh_filename = Misc::DirectoryFunctions::fromRelativeFilename(load_directory, mesh_filename);
			addPropMesh(mesh_name, mesh_filename);
			return true;
		}

		return false;
	}

	void HRMapObjects::saveToStream(std::ofstream& stream, const std::string& save_directory, Misc::ProgressTracker* progress_tracker) const {

		if (progress_tracker)
			progress_tracker->setCurrentStage("Saving map objects...");

		stream << "#Prop Meshes\n";
		for (unsigned int i = 0; i < propMeshNames.size(); i++) {
			stream << HRMAP_PROPMESH_LABEL << ' ' << propMeshNames[i] << ' '
				<< Misc::DirectoryFunctions::toRelativeFilename(save_directory, propMeshFilenames[i]) << '\n';
		}

		stream << "#OpenGL::Lights\n";
		for (unsigned int i = 0; i < lights.size(); i++) {

			if (progress_tracker)
				progress_tracker->incrementStep();

			stream << HRMAP_LIGHT_LABEL << ' ' << (*lights[i]) << '\n';
		}

		stream << "#Path Nodes\n";

		for (unsigned int i = 0; i < pathNodes.size(); i++)
			pathNodes[i]->index = i;

		for (unsigned int i = 0; i < pathNodes.size(); i++) {

			if (progress_tracker)
				progress_tracker->incrementStep();

			stream << HRMAP_PATHNODE_LABEL << ' ' << (*pathNodes[i])
				<< ' ' << pathNodes[i]->getProgress() << ' ';

			const std::vector<PathNode*>& next_nodes = pathNodes[i]->getNextNodes();
			stream << next_nodes.size() << ' ';
			for (unsigned int i = 0; i < next_nodes.size(); i++) {
				stream << next_nodes[i]->index;
				if (i < next_nodes.size()-1) {
					stream << ' ';
				}
			}
			stream << '\n';
		}

		stream << "#Start Points\n";
		for (unsigned int i = 0; i < startPoints.size(); i++) {

			if (progress_tracker)
				progress_tracker->incrementStep();

			stream << HRMAP_STARTPOINT_LABEL << ' ' << (*startPoints[i]) << '\n';
		}

		stream << "#Mesh Instances\n";
		for (unsigned int i = 0; i < meshInstances.size(); i++) {

			if (progress_tracker)
				progress_tracker->incrementStep();

			stream << HRMAP_MESHINSTANCE_LABEL << ' ' << (*meshInstances[i]) << '\n';
		}

	}

	void HRMapObjects::parseBegin() {
		pathnodeLinks.clear();
	}

	void HRMapObjects::parseFinish() {

		//Link all loaded path nodes
		for (unsigned int i = 0; i < pathnodeLinks.size(); i++) {
			for (unsigned int j = 0; j < pathnodeLinks[i].size(); j++) {
				pathNodes[i]->linkToNode(pathNodes[pathnodeLinks[i][j]]);
			}
		}

		pathnodeLinks.clear();
	}


}  // namespace Map
}  // namespace Project
