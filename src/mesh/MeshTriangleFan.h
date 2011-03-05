#ifndef PROJECT_RENDER__MESH_TRIANGLE_FAN_H
#define PROJECT_RENDER__MESH_TRIANGLE_FAN_H

#include "math/BoundingBox3D.h"
#include "MeshVertex.h"
#include <vector>

namespace Project {
namespace Mesh {

class MeshTriangleFan
	: public Math::BoundingBox3D {
private:
	std::vector<MeshVertex*> vertices;
	unsigned int displayList;
	int shaderIndex;
public:
	MeshTriangleFan(std::vector<MeshVertex*> _vertices);
	const std::vector<MeshVertex*>& getVertices() const { return vertices; }

	void setDisplayList(unsigned int display_list) { displayList = display_list; }
	unsigned int getDisplayList() const { return displayList; }

	void setShaderIndex(int shader_index) { shaderIndex = shader_index; }
	int getShaderIndex() const { return shaderIndex; }
};

}  // namespace Mesh
}  // namespace Project

#endif
