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

	Project::Math::Point normal;
	Project::Math::Point tangent;
	Project::Math::Point bitangent;
	float texCoordU;
	float texCoordV;

	std::vector<MeshTriangle*> adjacentTriangles;

public:

	MeshVertex(Project::Math::Point position, Project::Math::Point normal, Project::Math::Point tangent, Project::Math::Point bitangent,
		float texCoordU, float texCoordV);

	Project::Math::Point getNormal() const;
	Project::Math::Point getTangent() const;
	Project::Math::Point getBitangent() const;
	float getTexCoordU() const;
	float getTexCoordV() const;

	void addAdjacentTriangle(MeshTriangle* triangle);
	void removeAdjacentTriangle(MeshTriangle* triangle);
	const std::vector<MeshTriangle*>& getAdjacentTriangles() const { return adjacentTriangles; }
	int degree() const { return static_cast<int>(adjacentTriangles.size()); }


};

}  // namespace Mesh
}  // namespace Project

#endif
