#ifndef PROJECT_RENDER__MESH_VERTEX_H
#define PROJECT_RENDER__MESH_VERTEX_H

#include "math/Vertex3D.h"

namespace Project {
namespace Render {

class MeshVertex
	: public Project::Math::Vertex3D {
private:

	Project::Math::Point normal;
	Project::Math::Point tangent;
	Project::Math::Point bitangent;
	float texCoordU;
	float texCoordV;

public:

	MeshVertex(Project::Math::Point position, Project::Math::Point normal, Project::Math::Point tangent, Project::Math::Point bitangent,
		float texCoordU, float texCoordV);

	Project::Math::Point getNormal() const;
	Project::Math::Point getTangent() const;
	Project::Math::Point getBitangent() const;
	float getTexCoordU() const;
	float getTexCoordV() const;


};

}  // namespace Render
}  // namespace Project

#endif
