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
public:
	MeshTriangleFan(std::vector<MeshVertex*> _vertices);
	const std::vector<MeshVertex*>& getVertices() const { return vertices; }
};

}  // namespace Mesh
}  // namespace Project

#endif
