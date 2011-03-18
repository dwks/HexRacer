#include "MeshVertex.h"
#include "misc/StdVectorFunctions.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Mesh {

	MeshVertex::MeshVertex(const Point& position, const Point& normal, const Point& tangent, const Point& bitangent,
		float texCoordU, float texCoordV)
		: Vertex3D(position) {
		this->normal = normal;
		this->tangent = tangent;
		this->bitangent = bitangent;
		this->texCoordU = texCoordU;
		this->texCoordV = texCoordV;
	}

	void MeshVertex::addAdjacentTriangle(MeshTriangle* triangle) {
		adjacentTriangles.push_back(triangle);
	}
	void MeshVertex::removeAdjacentTriangle(MeshTriangle* triangle) {
		Misc::vectorRemoveOneElement(adjacentTriangles, triangle);
	}

}  // namespace Mesh
}  // namespace Project
