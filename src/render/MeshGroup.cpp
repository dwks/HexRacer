#include "MeshGroup.h"
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Render {

	MeshGroup::MeshGroup(string _name, vector<Mesh*> meshes, vector<MeshVertex*> _vertices,
		std::vector<Triangle3D>* collision_mask) {

		this->name = _name;
		for (unsigned int i = 0; i < meshes.size(); i++) {
			this->meshes.push_back(meshes[i]);
		}

		this->vertices = _vertices;
		if (vertices.size() > 0) {
			boundingBox.setToObject(*vertices[0]);
			for (unsigned int i = 1; i < vertices.size(); i++) {
				boundingBox.expandToInclude(*vertices[i]);
			}
		}

		collisionMask = collision_mask;
	}

	MeshGroup::~MeshGroup() {
		for (unsigned int i = 0; i < meshes.size(); i++)
			delete(meshes[i]);
		for (unsigned int i = 0; i < vertices.size(); i++)
			delete(vertices[i]);
		if (collisionMask)
			delete(collisionMask);
	}

	vector<RenderableObject*> MeshGroup::getChildren() {
		return meshes;
	}

	vector<Triangle3D> MeshGroup::getTriangles() {
		if (collisionMask)
			return *collisionMask;

		if (meshes.size() == 1) {
			return ((Mesh*)meshes[0])->getTriangles();
		}
		vector<Triangle3D> return_list;
		for (unsigned int i = 0; i < meshes.size(); i++) {
			vector<Triangle3D> child_triangles = ((Mesh*)meshes[i])->getTriangles();
			for (unsigned int j = 0; j < child_triangles.size(); j++) {
				return_list.push_back(child_triangles[j]);
			}
		}
		return return_list;
	}

}  // namespace Render
}  // namespace Project
