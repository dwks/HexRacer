#ifndef PROJECT_RENDER__MESH_GROUP_H
#define PROJECT_RENDER__MESH_GROUP_H

#include <vector>
#include <string>
#include "CompositeRenderable.h"
#include "Mesh.h"
#include "math/BoundingBox3D.h"

namespace Project {
namespace Render {

/** A group of triangle-meshes
*/
class MeshGroup
	: public CompositeRenderable {
private:

	std::string name;
	std::vector<RenderableObject*> meshes;
	std::vector<MeshVertex*> vertices;
	std::vector<Math::Triangle3D>* collisionMask;
	Math::BoundingBox3D boundingBox;

public:

	MeshGroup(std::string _name, std::vector<Mesh*> meshes, std::vector<MeshVertex*> _vertices,
		std::vector<Math::Triangle3D>* collison_mask = NULL);
	~MeshGroup();
	std::string getName() { return name; }
	const std::vector<RenderableObject*>& getChildren() { return meshes; }
	std::vector<Math::Triangle3D> getTriangles();
	Math::BoundingBox3D getBoundingBox() { return boundingBox; }

};

}  // namespace Render
}  // namespace Project

#endif
