#ifndef PROJECT_RENDER__MESH_GROUP_H
#define PROJECT_RENDER__MESH_GROUP_H

#include <vector>
#include <string>
#include "CompositeRenderable.h"
#include "Mesh.h"
#include "math/BoundingBox3D.h"
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
	Project::Math::BoundingBox3D boundingBox;

public:

	MeshGroup(string _name, vector<Mesh*> meshes, vector<MeshVertex*> _vertices);
	~MeshGroup();
	string getName() { return name; }
	vector<RenderableObject*> getChildren();
	vector<Project::Math::Triangle3D> getTriangles();
	Project::Math::BoundingBox3D getBoundingBox() { return boundingBox; }

};

}  // namespace Render
}  // namespace Project

#endif
