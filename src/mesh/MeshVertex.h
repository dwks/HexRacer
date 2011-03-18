#ifndef PROJECT_RENDER__MESH_VERTEX_H
#define PROJECT_RENDER__MESH_VERTEX_H

#include "math/Vertex3D.h"
#include <vector>

namespace Project {
namespace Mesh {

class MeshTriangle;

/** A vertex of a polygon mesh
*/
class MeshVertex
	: public Project::Math::Vertex3D {
private:

	Math::Point normal;
	Math::Point tangent;
	Math::Point bitangent;
	float texCoordU;
	float texCoordV;

	std::vector<MeshTriangle*> adjacentTriangles;

public:

	MeshVertex(const Math::Point& position, const Math::Point& normal, const Math::Point& tangent, const Math::Point& bitangent,
		float texCoordU, float texCoordV);

	const Math::Point& getNormal() const { return normal; }
	const Math::Point& getTangent() const { return tangent; }
	const Math::Point& getBitangent() const { return bitangent; }
	float getTexCoordU() const { return texCoordU; }
	float getTexCoordV() const { return texCoordV; }

	void addAdjacentTriangle(MeshTriangle* triangle);
	void removeAdjacentTriangle(MeshTriangle* triangle);
	const std::vector<MeshTriangle*>& getAdjacentTriangles() const { return adjacentTriangles; }
	int degree() const { return static_cast<int>(adjacentTriangles.size()); }


};

}  // namespace Mesh
}  // namespace Project

#endif
