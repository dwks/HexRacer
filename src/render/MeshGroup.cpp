#include "MeshGroup.h"
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Render {

	MeshGroup::MeshGroup(string name, vector<Mesh*> meshes) {

		this->name = name;
		for (unsigned int i = 0; i < meshes.size(); i++) {
			this->meshes.push_back(meshes[i]);
		}
	}

	string MeshGroup::getName() {
		return name;
	}

	vector<RenderableObject*> MeshGroup::getChildren() {
		return meshes;
	}

}  // namespace Render
}  // namespace Project
