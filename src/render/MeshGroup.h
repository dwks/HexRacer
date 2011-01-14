#ifndef PROJECT_RENDER__MESH_GROUP_H
#define PROJECT_RENDER__MESH_GROUP_H

#include <vector>
#include <string>
#include "CompositeRenderable.h"
#include "Mesh.h"
using namespace std;

namespace Project {
namespace Render {

class MeshGroup
	: public CompositeRenderable {
public:

	MeshGroup(string name, vector<Mesh*> meshes);
	string getName();
	vector<RenderableObject*> getChildren();


private:

	string name;
	vector<RenderableObject*> meshes;

};

}  // namespace Render
}  // namespace Project

#endif
