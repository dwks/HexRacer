#ifndef PROJECT_RENDER__MESH_TRIANGLE_H
#define PROJECT_RENDER__MESH_TRIANGLE_H

#include "math/Triangle3D.h"
#include "MeshVertex.h"

namespace Project {
namespace Render {

/** A triangle that is linked to mesh vertices
*/
class MeshTriangle
	: public Project::Math::Triangle3D {
private:

	MeshVertex* vertex [3];

public:

	MeshTriangle(MeshVertex* v1, MeshVertex* v2, MeshVertex* v3);

	Project::Math::Point getVertex(short vert_index) const;
	MeshVertex* getMeshVertex(short vert_index) const;
};

}  // namespace Render
}  // namespace Project

#endif