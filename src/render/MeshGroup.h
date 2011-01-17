#ifndef PROJECT_RENDER__MESH_GROUP_H
#define PROJECT_RENDER__MESH_GROUP_H

#include <vector>
#include <string>
#include "CompositeRenderable.h"
#include "Mesh.h"
using namespace std;

namespace Project {
namespace Render {

/** A group of triangle-meshes
*/
class MeshGroup
	: public CompositeRenderable {
private:

	string name;
	vector<RenderableObject*> meshes;
	vector<MeshVertex*> vertices;

public:

	MeshGroup(string name, vector<Mesh*> meshes, vector<MeshVertex*> vertices);
	~MeshGroup();
	string getName() { return name; }
	vector<RenderableObject*> getChildren();
	vector<Project::Math::Triangle3D> getTriangles();

};

}  // namespace Render
}  // namespace Project

#endif
