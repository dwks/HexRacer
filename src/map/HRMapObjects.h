#ifndef PROJECT_MAP__HRMAP_OBJECTS_H
#define PROJECT_MAP__HRMAP_OBJECTS_H

#include <vector>
#include "opengl/Light.h"
#include "PathNode.h"
#include "MeshInstance.h"
#include "misc/ProgressTracker.h"
#include <fstream>

#define HRMAP_PAINTCELL_LABEL "paintCell"
#define HRMAP_LIGHT_LABEL "light"
#define HRMAP_PATHNODE_LABEL "pathNode"
#define HRMAP_STARTPOINT_LABEL "startPoint"
#define HRMAP_MESHINSTANCE_LABEL "meshInstance"
#define HRMAP_PROPMESH_LABEL "propMesh"

namespace Project {
namespace Map {

class HRMapObjects {
private:

	std::vector<OpenGL::Light*> lights;
	std::vector<PathNode*> pathNodes;
	std::vector<Math::Vertex3D*> startPoints;
	std::vector<MeshInstance*> meshInstances;

	std::vector<std::string> propMeshNames;
	std::vector<std::string> propMeshFilenames;

	std::vector< std::vector<int> > pathnodeLinks;

public:

	void clear();

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

	const std::vector<MeshInstance*> getMeshInstances() const { return meshInstances; }
	bool addMeshInstance(MeshInstance* mesh);
	void removeMeshInstance(MeshInstance* mesh);
	void clearMeshInstances();

	void scaleAll(double scale, Math::Point origin = Math::Point());
	void translateAll(Math::Point translation);

	bool parseStream(const std::string& keyword, std::ifstream& stream,
		const std::string& load_directory, const std::string& version);
	void saveToStream(std::ofstream& stream, const std::string& save_directory,
		Misc::ProgressTracker* progress_tracker = NULL) const;
	void parseBegin();
	void parseFinish();

	unsigned int getNumObjects() const {
		return lights.size()+pathNodes.size()+startPoints.size()+meshInstances.size();
	}
};

}  // namespace Map
}  // namespace Project

#endif
