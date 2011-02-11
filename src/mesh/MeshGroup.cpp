#include "MeshGroup.h"
#include "render/RenderManager.h"
#include "math/Values.h"
using namespace Project;
using namespace Math;
using namespace std;

namespace Project {
namespace Mesh {

	MeshGroup::MeshGroup(string _name, vector<SubMesh*> _meshes, vector<MeshVertex*> _vertices,
		std::vector<Triangle3D> collision_mask) {

		this->name = _name;
	
		meshes = _meshes;

		this->vertices = _vertices;
		radiusFromOrigin = 0.0;
		for (unsigned int i = 0; i < vertices.size(); i++) {
			if (i == 0)
				boundingBox.setToObject(*vertices[0]);
			else
				boundingBox.expandToInclude(*vertices[i]);
			radiusFromOrigin = Math::maximum(radiusFromOrigin, _vertices[i]->getPosition().lengthSquared());
		}

		radiusFromOrigin = sqrt(radiusFromOrigin);

		collisionMask = collision_mask;
	}

	MeshGroup::~MeshGroup() {
		for (unsigned int i = 0; i < meshes.size(); i++)
			delete(meshes[i]);
		for (unsigned int i = 0; i < vertices.size(); i++)
			delete(vertices[i]);
	}

	vector<Triangle3D> MeshGroup::getTriangles() {
		if (!collisionMask.empty())
			return collisionMask;

		if (meshes.size() == 1) {
			return meshes[0]->getTriangles();
		}

		vector<Triangle3D> return_list;
		for (unsigned int i = 0; i < meshes.size(); i++) {
			const vector<Triangle3D>& mesh_triangles = meshes[i]->getTriangles();
			return_list.insert(return_list.end(), mesh_triangles.begin(), mesh_triangles.end());
		}
		return return_list;
	}

	void MeshGroup::subRender(Render::RenderManager* manager) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i]->render(manager);
		}
	}

}  // namespace Mesh
}  // namespace Project
