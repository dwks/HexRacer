#ifndef PROJECT_RENDER__MESH_GROUP_H
#define PROJECT_RENDER__MESH_GROUP_H

#include <vector>
#include <string>
#include "render/RenderableObject.h"
#include "SubMesh.h"
#include "math/BoundingBox3D.h"

namespace Project {
namespace Mesh {

/** A group of triangle-meshes
*/
class MeshGroup
	: public Render::RenderableObject {
private:

	std::string name;

	std::vector<SubMesh*> meshes;
	std::vector<MeshVertex*> vertices;
	std::vector<Math::Triangle3D> collisionMask;

	Math::BoundingBox3D boundingBox;
	double radiusFromOrigin;

public:

	MeshGroup(std::string _name, std::vector<SubMesh*> _meshes, std::vector<MeshVertex*> _vertices,
		std::vector<Math::Triangle3D> collison_mask = std::vector<Math::Triangle3D>());
	~MeshGroup();
	std::string getName() { return name; }
	void subRender(Render::RenderManager* manager);
	std::vector<Math::Triangle3D> getTriangles();
	Math::BoundingBox3D getBoundingBox() { return boundingBox; }
	double getRadiusFromOrigin() { return radiusFromOrigin; }

};

}  // namespace Mesh
}  // namespace Project

#endif
